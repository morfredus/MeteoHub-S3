#include <cctype>
#include <string>
#include <ArduinoJson.h>
#include <HTTPClient.h>

#include "forecast_manager.h"
#include "secrets.h"
#include "../utils/logs.h"

// Mettre à jour toutes les 30 minutes
const unsigned long UPDATE_INTERVAL = 30 * 60 * 1000;

void ForecastManager::begin() {
    lastUpdate = -UPDATE_INTERVAL; // Force an update on first run
}

void ForecastManager::update() {
    unsigned long now = millis();
    if (now - lastUpdate < UPDATE_INTERVAL) {
        return;
    }
    lastUpdate = now;

    LOG_DEBUG("Fetching forecast...");

    HTTPClient http;
    std::string url = "https://api.openweathermap.org/data/3.0/onecall?lat=";
    url += OWM_LAT;
    url += "&lon=";
    url += OWM_LON;
    url += "&appid=";
    url += OWM_API_KEY;
    url += "&units=";
    url += OWM_UNITS;
    url += "&lang=";
    url += OWM_LANG;
    url += "&exclude=minutely,hourly,current";

    http.begin(url.c_str());
    int httpCode = http.GET();

    if (httpCode == HTTP_CODE_OK) {
        std::string payload = http.getString().c_str();
        parseResponse(payload);
        LOG_INFO("Forecast OK");
    } else {
        LOG_WARNING(std::string("Forecast fail: ") + std::to_string(httpCode));
    }

    http.end();
}

void ForecastManager::parseResponse(const std::string& payload) {
    DynamicJsonDocument doc(8192); // Allocate enough memory for the response
    DeserializationError error = deserializeJson(doc, payload.c_str());

    if (error) {
        LOG_ERROR("JSON parse err");
        return;
    }

    // Today's Forecast (index 0 of daily array)
    JsonObject daily_0 = doc["daily"][0];
    today.temp_min = daily_0["temp"]["min"];
    today.temp_max = daily_0["temp"]["max"];
    today.description = daily_0["weather"][0]["description"].as<const char*>();
    if (!today.description.empty()) {
        today.description[0] = static_cast<char>(std::toupper(static_cast<unsigned char>(today.description[0])));
    }

    // Tomorrow's Forecast (index 1 of daily array)
    JsonObject daily_1 = doc["daily"][1];
    tomorrow.temp_min = daily_1["temp"]["min"];
    tomorrow.temp_max = daily_1["temp"]["max"];
    tomorrow.description = daily_1["weather"][0]["description"].as<const char*>();
    if (!tomorrow.description.empty()) {
        tomorrow.description[0] = static_cast<char>(std::toupper(static_cast<unsigned char>(tomorrow.description[0])));
    }

    // Alerts
    if (doc.containsKey("alerts")) {
        alert_active = true;
        alert.sender = doc["alerts"][0]["sender_name"].as<const char*>();
        alert.event = doc["alerts"][0]["event"].as<const char*>();
        
        // Tentative de déduction de la sévérité basée sur le texte (simplifié)
        std::string evt = alert.event;
        // Conversion en minuscules pour la recherche
        for (auto& c : evt) c = std::tolower((unsigned char)c);

        if (evt.find("rouge") != std::string::npos || evt.find("red") != std::string::npos) alert.severity = 3;
        else if (evt.find("orange") != std::string::npos) alert.severity = 2;
        else alert.severity = 1; // Jaune par défaut

    } else {
        alert_active = false;
        alert.severity = 0;
    }
}