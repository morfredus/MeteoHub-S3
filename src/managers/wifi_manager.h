#pragma once
#include <string>
 
class WifiManager {
public:
    void begin();
    void update();

    std::string ssid() const { return currentSSID; }
    std::string ip() const;
    int rssi() const;

private:
    std::string currentSSID;
    unsigned long lastAttempt = 0;
};
