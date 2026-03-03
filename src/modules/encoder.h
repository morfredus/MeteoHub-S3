#pragma once
#include <Arduino.h>
#include <ESP32Encoder.h>
#include "board_config.h"
 
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
#if defined(ENCODER_MODEL_EC11)
    // EC11 (module SH1106): conserver le comportement actuel (1 cran = 1 changement)
    static constexpr int8_t COUNTS_PER_DETENT = 4;
#else
    // KY-040: sensibilité augmentée pour obtenir 1 cran = 1 changement (au lieu de 2)
    static constexpr int8_t COUNTS_PER_DETENT = 2;
#endif
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
