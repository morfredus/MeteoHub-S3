#pragma once

#include "display_interface.h"
#if defined(ESP32_S3_OLED)
#include "../managers/forecast_manager.h"
#include "../managers/history.h"
#include "../managers/wifi_manager.h"
#include "sensors.h"
#include "../utils/system_info.h"

void pageNetwork_sh1106(DisplayInterface& d, WifiManager& wifi, int pageIndex, int pageCount);
void pageSystem_sh1106(DisplayInterface& d, int pageIndex, int pageCount);
void pageLogs_sh1106(DisplayInterface& d, int pageIndex, int pageCount);
void pageWeather_sh1106(DisplayInterface& d, SensorManager& sensors, int pageIndex, int pageCount);
void pageGraph_sh1106(DisplayInterface& d, HistoryManager& history, int type, int pageIndex, int pageCount);
void pageForecast_sh1106(DisplayInterface& d, ForecastManager& forecast, int view, int pageIndex, int pageCount);
#endif
