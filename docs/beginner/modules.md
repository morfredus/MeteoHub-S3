# MeteoHub Project Modules

## Detailed Example: Sensor Module

```cpp
// src/modules/sensors.h
class Sensors {
public:
    Sensors();
    float getTemperature();
    float getHumidity();
};

// src/modules/sensors.cpp
#include "sensors.h"
Sensors::Sensors() {
    // Sensor initialization
}
float Sensors::getTemperature() {
    // Code to read temperature
    return 23.7;
}
float Sensors::getHumidity() {
    // Code to read humidity
    return 55.2;
}
```

## Detailed Example: Usage in main.cpp

```cpp
#include "modules/sensors.h"
Sensors sensors;
void setup() {
    Serial.begin(115200);
}
void loop() {
    float t = sensors.getTemperature();
    float h = sensors.getHumidity();
    Serial.print("Temperature: ");
    Serial.println(t);
    Serial.print("Humidity: ");
    Serial.println(h);
    delay(2000);
}
```
