#pragma once
#include <Arduino.h>

struct SystemInfo {
    uint32_t heapFree;
    uint32_t psramFree;
    uint32_t flashSize;
};

SystemInfo getSystemInfo();

//