#include "system.h"
#include <Esp.h>

SystemInfo getSystemInfo() {
    SystemInfo info;
    info.heapFree = ESP.getFreeHeap();
    info.psramFree = ESP.getFreePsram();
    info.flashSize = ESP.getFlashChipSize();
    return info;
}

//