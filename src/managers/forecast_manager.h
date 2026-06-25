#pragma once
#include <string>

struct Forecast {
    // Daily
    float temp_day;
    float temp_min;
    float temp_max;
    std::string description;
    int weather_id;
};

struct Alert {
    std::string sender;
    std::string event;
    std::string description;
    int severity; // 1=Yellow, 2=Orange, 3=Red
    long start_unix = 0;
    long end_unix = 0;
};

// Prévision journalière étendue (utilisée pour le résumé 7 jours de la page Tendances).
struct DailyForecast {
    long dt = 0;
    float temp_min = 0.0f;
    float temp_max = 0.0f;
    std::string description;
    int weather_id = 0;
    float pop = 0.0f;       // Probabilité de précipitation, 0..1
    float rain_mm = 0.0f;
    float wind_speed = 0.0f; // m/s
    int wind_deg = 0;
};

static const int FORECAST_DAILY_MAX = 8; // L'API OWM One Call renvoie jusqu'à 8 jours (aujourd'hui + 7).

class ForecastManager {
public:
    void begin();
    void update();

    Forecast today;
    Forecast tomorrow;
    Alert alert;
    bool alert_active = false;

    DailyForecast daily[FORECAST_DAILY_MAX];
    int daily_count = 0;

private:
    unsigned long lastUpdate = 0;
    void parseResponse(const std::string& payload);
};