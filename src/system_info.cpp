#include "system_info.h"
#include <Esp.h>

SystemInfo getSystemInfo() {
    SystemInfo s;
    s.heapFree  = ESP.getFreeHeap();
    s.psramFree = ESP.getFreePsram();
    s.flashSize = ESP.getFlashChipSize();
    s.cpuFreq   = ESP.getCpuFreqMHz();
    s.uptime    = millis() / 1000;
    return s;
}
