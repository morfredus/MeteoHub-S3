#pragma once
#include <Arduino.h>

struct Forecast {
    // Daily
    float temp_day;
    float temp_min;
    float temp_max;
    String description;
    int weather_id;
};

struct Alert {
    String sender;
    String event;
};

class ForecastManager {
public:
    void begin();
    void update();

    Forecast today;
    Forecast tomorrow;
    Alert alert;
    bool alert_active = false;

private:
    unsigned long lastUpdate = 0;
    void parseResponse(const String& payload);
};