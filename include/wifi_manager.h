#pragma once
#include <Arduino.h>

class WifiManager {
public:
    void begin();
    void update();

    String ssid() const { return currentSSID; }
    String ip() const;
    int rssi() const;

private:
    String currentSSID;
    unsigned long lastAttempt = 0;
};
