#pragma once
#include "display_interface.h"
#include "../managers/wifi_manager.h"
#include "sensors.h"
#include "../managers/forecast_manager.h"
#include "../managers/history_manager.h"

void pageNetwork_sh1106(DisplayInterface& d, WifiManager& wifi, int pageIndex, int pageCount);
void pageSystem_sh1106(DisplayInterface& d, int pageIndex, int pageCount);
void pageLogs_sh1106(DisplayInterface& d, int pageIndex, int pageCount);
void pageWeather_sh1106(DisplayInterface& d, SensorManager& sensors, int pageIndex, int pageCount);
void pageGraph_sh1106(DisplayInterface& d, HistoryManager& history, int type, int pageIndex, int pageCount);
void pageForecast_sh1106(DisplayInterface& d, ForecastManager& forecast, int view, int pageIndex, int pageCount);
void drawSplashScreen_sh1106(DisplayInterface& d);
void drawBootProgress_sh1106(DisplayInterface& d, int step, int total, const std::string& msg);