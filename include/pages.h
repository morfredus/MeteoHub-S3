#pragma once
#include "sh1106_display.h"
#include "wifi_manager.h"
#include "system_info.h"
#include "sensors.h"
#include "history.h"
#include "forecast_manager.h"

void pageNetwork(Sh1106Display& d, WifiManager& wifi, int pageIndex, int pageCount);
void pageSystem(Sh1106Display& d, int pageIndex, int pageCount);
void pageLogs(Sh1106Display& d, int pageIndex, int pageCount);
void pageWeather(Sh1106Display& d, SensorManager& sensors, int pageIndex, int pageCount);
void pageGraph(Sh1106Display& d, HistoryManager& history, int type, int pageIndex, int pageCount);
void pageForecast(Sh1106Display& d, ForecastManager& forecast, int view, int pageIndex, int pageCount);
