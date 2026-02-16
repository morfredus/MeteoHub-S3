#pragma once
#include "../managers/forecast_manager.h"
#include "../managers/history.h"
#include "../managers/wifi_manager.h"
#include "sensors.h"
#include "../utils/system_info.h"

// Interface d'affichage générique
class DisplayInterface {
public:
	virtual ~DisplayInterface() = default;
	virtual bool begin() = 0;
	virtual void clear() = 0;
	virtual void show() = 0;
	virtual void text(int x, int y, const std::string& s) = 0;
	virtual void center(int y, const std::string& s) = 0;
	virtual void bar(int x, int y, int w, int h, int value, int max) = 0;
	virtual void drawLine(int x0, int y0, int x1, int y1) = 0;
};

void pageNetwork(DisplayInterface& d, WifiManager& wifi, int pageIndex, int pageCount);
void pageSystem(DisplayInterface& d, int pageIndex, int pageCount);
void pageLogs(DisplayInterface& d, int pageIndex, int pageCount);
void pageWeather(DisplayInterface& d, SensorManager& sensors, int pageIndex, int pageCount);
void pageGraph(DisplayInterface& d, HistoryManager& history, int type, int pageIndex, int pageCount);
void pageForecast(DisplayInterface& d, ForecastManager& forecast, int view, int pageIndex, int pageCount);
