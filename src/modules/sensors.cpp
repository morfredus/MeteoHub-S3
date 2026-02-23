#include <Wire.h>
#include "board_config.h"
#include "sensors.h"
#include "../utils/logs.h"

bool SensorManager::begin() {
    Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);

    // Initialisation AHT20 (Adresse 0x38 par défaut)
    if (aht.begin()) {
        ahtFound = true;
    } else {
        ahtFound = false;
    }

    // Initialisation BMP280 (Adresse 0x77 ou 0x76)
    if (bmp.begin(0x77)) {
        bmpFound = true;
    } else if (bmp.begin(0x76)) {
        bmpFound = true;
    } else {
        bmpFound = false;
    }

    if (ahtFound || bmpFound) {
        LOG_INFO("Sensors OK");
    } else {
        LOG_WARNING("Sensors Missing");
    }

    return ahtFound || bmpFound;
}

SensorData SensorManager::read() {
    SensorData data = {0, 0, 0, false};
    
    if (ahtFound) {
        sensors_event_t humidity, temp;
        aht.getEvent(&humidity, &temp);
        data.temperature = temp.temperature;
        data.humidity = humidity.relative_humidity;
        data.valid = true;
    }

    if (bmpFound) {
        data.pressure = bmp.readPressure() / 100.0F; // Conversion Pa -> hPa
        // Si l'AHT20 est absent, on utilise le thermomètre du BMP280
        if (!ahtFound) {
            data.temperature = bmp.readTemperature();
            data.valid = true;
        }
    }

    return data;
}

//