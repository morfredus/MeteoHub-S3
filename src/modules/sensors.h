#pragma once
#include <Adafruit_AHTX0.h>
#include <Adafruit_BMP280.h>

struct SensorData {
    float temperature;
    float humidity;
    float pressure;
    bool valid;
};

class SensorManager {
public:
    bool begin();
    SensorData read();

private:
    Adafruit_AHTX0 aht;
    Adafruit_BMP280 bmp;
    bool ahtFound = false;
    bool bmpFound = false;
};