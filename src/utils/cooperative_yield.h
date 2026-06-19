#pragma once

#include <Arduino.h>
#include <cstddef>

inline void cooperativeYieldEvery(std::size_t iteration, std::size_t period) {
    if (period == 0) {
        return;
    }

    if ((iteration % period) == 0) {
        delay(0);
    }
}

#define COOPERATIVE_YIELD_EVERY(iteration, period) cooperativeYieldEvery((iteration), (period))
