#include "forecast_manager.h"
#include "secrets.h"
#include "logs.h"
#include <HTTPClient.h>
#include <ArduinoJson.h>

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

    addLog("Fetching forecast...");

    HTTPClient http;
    String url = "https://api.openweathermap.org/data/3.0/onecall?lat=";
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

    http.begin(url);
    int httpCode = http.GET();

    if (httpCode == HTTP_CODE_OK) {
        String payload = http.getString();
        parseResponse(payload);
        addLog("Forecast OK");
    } else {
        addLog("Forecast fail: " + String(httpCode));
    }

    http.end();
}

void ForecastManager::parseResponse(const String& payload) {
    DynamicJsonDocument doc(8192); // Allocate enough memory for the response
    DeserializationError error = deserializeJson(doc, payload);

    if (error) {
        addLog("JSON parse err");
        return;
    }

    // Today's Forecast (index 0 of daily array)
    JsonObject daily_0 = doc["daily"][0];
    today.temp_min = daily_0["temp"]["min"];
    today.temp_max = daily_0["temp"]["max"];
    today.description = daily_0["weather"][0]["description"].as<String>();
    today.description.capitalize();

    // Tomorrow's Forecast (index 1 of daily array)
    JsonObject daily_1 = doc["daily"][1];
    tomorrow.temp_min = daily_1["temp"]["min"];
    tomorrow.temp_max = daily_1["temp"]["max"];
    tomorrow.description = daily_1["weather"][0]["description"].as<String>();
    tomorrow.description.capitalize();

    // Alerts
    if (doc.containsKey("alerts")) {
        alert_active = true;
        alert.sender = doc["alerts"][0]["sender_name"].as<String>();
        alert.event = doc["alerts"][0]["event"].as<String>();
    } else {
        alert_active = false;
    }
}