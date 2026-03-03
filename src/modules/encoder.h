#pragma once
#include <Arduino.h>
#include <ESP32Encoder.h>
#include "board_config.h"
#include "config.h"
 
class Encoder {

public:
    void begin();
    void update();

    bool rotatedCW();
    bool rotatedCCW();
    bool clicked();
    void clearQueue();
    int getStepCount() const;

private:
    // 1 cran = 1 changement sur les deux profils matériels.
    static constexpr int8_t COUNTS_PER_DETENT = 1;
    // Débounce rotation plus strict sur SH1106+EC11, plus léger sur SSD1306+KY-040.
    static constexpr uint16_t ROTATION_EVENT_DEBOUNCE_MS =
        (OLED_CONTROLLER == OLED_CTRL_SH1106) ? 8 : 4;
    static constexpr uint16_t CLICK_DEBOUNCE_MS = 120;
    static constexpr int16_t STEP_QUEUE_LIMIT = 20;
    static constexpr uint16_t ROTATION_EVENT_DEBOUNCE_MS = 8;

    ESP32Encoder rotary;
    int64_t lastCount = 0;
    int32_t pulseAccumulator = 0;
    int16_t stepQueue = 0;

    bool lastBtn = false;
    bool clickFlag = false;
    unsigned long lastClickMs = 0;
    unsigned long lastStepEventMs = 0;
};
