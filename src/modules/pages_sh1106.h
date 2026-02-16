#pragma once

#include "display_interface.h"
#if defined(ESP32_S3_OLED)
#include "../managers/forecast_manager.h"
#include "../managers/history.h"
#include "../managers/wifi_manager.h"
#include "sensors.h"
#include "../utils/system_info.h"

void pageNetwork(DisplayInterface& d, WifiManager& wifi, int pageIndex, int pageCount);
void pageSystem(DisplayInterface& d, int pageIndex, int pageCount);
void pageLogs(DisplayInterface& d, int pageIndex, int pageCount);
void pageWeather(DisplayInterface& d, SensorManager& sensors, int pageIndex, int pageCount);
void pageGraph(DisplayInterface& d, HistoryManager& history, int type, int pageIndex, int pageCount);
void pageForecast(DisplayInterface& d, ForecastManager& forecast, int view, int pageIndex, int pageCount);
#endif
