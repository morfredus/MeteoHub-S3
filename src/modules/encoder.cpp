#include "encoder.h"
#include "board_config.h"
#include "config.h"

int Encoder::getStepCount() const {
    return stepQueue;
}

void Encoder::begin() {
    pinMode(ENCODER_BTN_PIN, INPUT_PULLUP);

    ESP32Encoder::useInternalWeakPullResistors = puType::up;

#if OLED_CONTROLLER == OLED_CTRL_SH1106
    // Correction 10: profil SH1106+EC11 dédié (module intégré), anti-bruit renforcé.
    rotary.attachHalfQuad(ENCODER_A_PIN, ENCODER_B_PIN);
    rotary.setFilter(1023);
#else
    rotary.attachFullQuad(ENCODER_A_PIN, ENCODER_B_PIN);
    rotary.setFilter(120);
#endif
    rotary.clearCount();

    lastCount = 0;
    pulseAccumulator = 0;
    stepQueue = 0;
    lastStepEventMs = 0;
}

void Encoder::update() {
    int64_t currentCount = rotary.getCount();
    int32_t delta = static_cast<int32_t>(currentCount - lastCount);
    lastCount = currentCount;

    if (delta != 0) {
        const unsigned long now = millis();

        // Correction 11: anti-rafale logiciel des impulsions encodeur pour éviter
        // les bonds parasites qui saturent l'UI/OLED sur le module EC11+SH1106.
        if (lastStepEventMs != 0 && (now - lastStepEventMs) < rotationEventDebounceMs()) {
            delta = 0;
        }

        if (delta != 0) {
            if (delta > STEP_QUEUE_LIMIT) delta = STEP_QUEUE_LIMIT;
            if (delta < -STEP_QUEUE_LIMIT) delta = -STEP_QUEUE_LIMIT;

            pulseAccumulator += delta;

            while (pulseAccumulator >= COUNTS_PER_DETENT) {
                stepQueue++;
                pulseAccumulator -= COUNTS_PER_DETENT;
                lastStepEventMs = now;
            }

            while (pulseAccumulator <= -COUNTS_PER_DETENT) {
                stepQueue--;
                pulseAccumulator += COUNTS_PER_DETENT;
                lastStepEventMs = now;
            }

            if (stepQueue > STEP_QUEUE_LIMIT) stepQueue = STEP_QUEUE_LIMIT;
            if (stepQueue < -STEP_QUEUE_LIMIT) stepQueue = -STEP_QUEUE_LIMIT;
        }
    }

    bool btn = digitalRead(ENCODER_BTN_PIN) == LOW;
    unsigned long now = millis();
    if (btn && !lastBtn && (now - lastClickMs >= CLICK_DEBOUNCE_MS)) {
        clickFlag = true;
        lastClickMs = now;
    }
    lastBtn = btn;

}

bool Encoder::rotatedCW() {
    if (stepQueue > 0) {
        stepQueue--;
        return true;
    }

    return false;
}

bool Encoder::rotatedCCW() {
    if (stepQueue < 0) {
        stepQueue++;
        return true;
    }

    return false;
}

bool Encoder::clicked() {
    if (clickFlag) { clickFlag = false; return true; }
    return false;
}

void Encoder::clearQueue() {
    stepQueue = 0;
    pulseAccumulator = 0;
}

//
