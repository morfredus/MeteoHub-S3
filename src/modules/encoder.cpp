#include "encoder.h"
#include "board_config.h"

int Encoder::getStepCount() const {
    return stepQueue;
}

void Encoder::begin() {
    pinMode(ENCODER_BTN_PIN, INPUT_PULLUP);

    ESP32Encoder::useInternalWeakPullResistors = puType::up;

#if defined(ENCODER_MODEL_EC11)
    // Correction 8: EC11 sur module SH1106 "all-in-one" = bruit mécanique élevé.
    // HalfQuad + filtre IRQ plus fort pour limiter les tempêtes d'interruptions.
    rotary.attachHalfQuad(ENCODER_A_PIN, ENCODER_B_PIN);
    rotary.setFilter(1023);
#else
    rotary.attachFullQuad(ENCODER_A_PIN, ENCODER_B_PIN);
    rotary.setFilter(200);
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

        // Correction 9: anti-rafale logiciel des impulsions encodeur pour éviter
        // les bonds parasites qui saturent l'UI/OLED sur le module EC11+SH1106.
        if (lastStepEventMs != 0 && (now - lastStepEventMs) < ROTATION_EVENT_DEBOUNCE_MS) {
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
