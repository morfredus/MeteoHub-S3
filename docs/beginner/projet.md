# MeteoHub Project Structure

## Main Files
- `main.cpp`: program entry point, main loop.
- `config.h`: user configuration (modifiable parameters).
- `board_config.h`: hardware configuration (GPIO, etc).
- `secrets.h`: sensitive information (WiFi, API).

## Detailed Example: main.cpp
```cpp
#include <Arduino.h>
#include "config.h"
#include "modules/sensors.h"

Sensors sensors;

void setup() {
    Serial.begin(115200);
}

void loop() {
    float temp = sensors.getTemperature();
    Serial.print("Temperature: ");
    Serial.println(temp);
    delay(1000);
}
```

## Folder Organization
- `src/`: main source code
- `include/`: shared header files
- `lib/`: third-party libraries
- `data/`: embedded web files
- `docs/`: documentation
