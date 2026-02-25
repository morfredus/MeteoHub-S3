# C++ Basics for MeteoHub

## Variables
```cpp
int temperature = 25; // Declare an integer variable
float humidity = 60.5; // Floating point variable
```

## Functions
```cpp
float calculateAverage(float a, float b) {
    return (a + b) / 2.0;
}

void displayValue(float value) {
    Serial.print("Value: ");
    Serial.println(value);
}
```

## Classes
```cpp
class Sensor {
public:
    Sensor(int pin) : pin_(pin) {}
    float read() {
        // Simulate a reading
        return 42.0;
    }
private:
    int pin_;
};

Sensor tempSensor(4);
float t = tempSensor.read();
```

## Control Structures
```cpp
if (temperature > 30) {
    // Action if temperature is high
} else {
    // Otherwise
}

for (int i = 0; i < 10; i++) {
    // Loop 10 times
}
```

## Arrays
```cpp
float measures[5] = {22.1, 23.5, 24.0, 25.2, 26.3};
for (int i = 0; i < 5; i++) {
    Serial.println(measures[i]);
}
```

## Using a library
```cpp
#include <Wire.h>
#include "sensors.h"

Sensors sensors;
float temp = sensors.getTemperature();
```
