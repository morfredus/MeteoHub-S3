#pragma once
#include "display_interface.h"
#include "../managers/wifi_manager.h"
#include "sensors.h"
#include "../managers/forecast_manager.h"
#include "../managers/history_manager.h"

//
// Page functions
//
void pageNetwork_st7789(DisplayInterface& d, WifiManager& wifi, int pageIndex, int pageCount);
void pageSystem_st7789(DisplayInterface& d, int pageIndex, int pageCount);
void pageLogs_st7789(DisplayInterface& d, int pageIndex, int pageCount);
void pageWeather_st7789(DisplayInterface& d, SensorManager& sensors, int pageIndex, int pageCount);
void pageGraph_st7789(DisplayInterface& d, HistoryManager& history, int type, int pageIndex, int pageCount);
void pageForecast_st7789(DisplayInterface& d, ForecastManager& forecast, int view, int pageIndex, int pageCount);
void drawSplashScreen_st7789(DisplayInterface& d);
void drawBootProgress_st7789(DisplayInterface& d, int step, int total, const std::string& msg);