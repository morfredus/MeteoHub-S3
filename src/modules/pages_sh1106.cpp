#if defined(ESP32_S3_OLED)
#include "display_interface.h"
#include <algorithm>
#include <cctype>
#include <float.h>
#include <iomanip>
#include <sstream>
#include <string>
#include <time.h>
#include "pages_sh1106.h"
#include "../utils/logs.h"

std::string formatFloat(float value, int precision) {
	std::ostringstream stream;
	stream << std::fixed << std::setprecision(precision) << value;
	return stream.str();
}

std::string getHeader(const std::string& title, int pageIndex, int pageCount) {
	struct tm timeinfo;
	char timeStr[6];
	if (getLocalTime(&timeinfo)) {
		strftime(timeStr, sizeof(timeStr), "%H:%M", &timeinfo);
	} else {
		strcpy(timeStr, "--:--");
	}
	return title + " " + std::string(timeStr) + " (" + std::to_string(pageIndex) + "/" + std::to_string(pageCount) + ")";
}

std::string translateAlert(const std::string& event) {
	std::string lowerEvent = event;
	std::transform(lowerEvent.begin(), lowerEvent.end(), lowerEvent.begin(), [](unsigned char character) {
		return static_cast<char>(std::tolower(character));
	});

	if (lowerEvent.find("crue") != std::string::npos) return "Vigilance Crues";
	if (lowerEvent.find("flood") != std::string::npos || lowerEvent.find("inond") != std::string::npos) return "Vigilance Inondation";
	if (lowerEvent.find("rain") != std::string::npos || lowerEvent.find("pluie") != std::string::npos) return "Vigilance Pluie";
	if (lowerEvent.find("wind") != std::string::npos || lowerEvent.find("vent") != std::string::npos) return "Vigilance Vent";
	if (lowerEvent.find("thunderstorm") != std::string::npos) return "Vigilance Orages";
	if (lowerEvent.find("snow") != std::string::npos) return "Vigilance Neige";
	if (lowerEvent.find("heat") != std::string::npos) return "Vigilance Canicule";
	if (lowerEvent.find("cold") != std::string::npos) return "Vigilance Grand Froid";
	if (lowerEvent.find("ice") != std::string::npos) return "Vigilance Verglas";
	return event; // Fallback si aucune traduction
}

void pageNetwork_sh1106(DisplayInterface& d, WifiManager& wifi, int pageIndex, int pageCount) {
	d.clear();
	d.text(0, 0, getHeader("Net.", pageIndex, pageCount));

	d.text(0, 16, std::string("SSID: ") + wifi.ssid());
	d.text(0, 28, std::string("IP:   ") + wifi.ip());
	d.text(0, 40, std::string("RSSI: ") + std::to_string(wifi.rssi()) + " dBm");

	d.show();
}

void pageSystem_sh1106(DisplayInterface& d, int pageIndex, int pageCount) {
	SystemInfo s = getSystemInfo();

	d.clear();
	d.text(0, 0, getHeader("Sys.", pageIndex, pageCount));

	d.text(0, 16, std::string("Heap:  ") + std::to_string(s.heapFree / 1024) + " KB");
	d.text(0, 28, std::string("PSRAM: ") + std::to_string(s.psramFree / 1024) + " KB");
	d.text(0, 40, std::string("Flash: ") + std::to_string(s.flashSize / 1024 / 1024) + " MB");
	d.text(0, 52, std::string("Ver:   ") + std::string(PROJECT_VERSION));

	d.show();
}

void pageLogs_sh1106(DisplayInterface& d, int pageIndex, int pageCount) {
	d.clear();
	d.text(0, 0, getHeader("Logs", pageIndex, pageCount));

	int y = 12;
	for (int i = 0; i < getLogCount(); i++) {
		d.text(0, y, getLog(i));
		y += 10;
		if (y > 54) break;
	}

	d.show();
}

void pageWeather_sh1106(DisplayInterface& d, SensorManager& sensors, int pageIndex, int pageCount) {
	SensorData data = sensors.read();
	d.clear();
	d.text(0, 0, getHeader("Meteo", pageIndex, pageCount));

	if (data.valid) {
		d.text(0, 16, std::string("Temp: ") + formatFloat(data.temperature, 1) + " C");
		d.text(0, 28, std::string("Hum:  ") + formatFloat(data.humidity, 0) + " %");
		d.text(0, 40, std::string("Pres: ") + formatFloat(data.pressure, 0) + " hPa");
	} else {
		d.center(30, "AHT20 / BMP280");
		d.center(42, "Non detecte");
	}
	d.show();
}

void pageGraph_sh1106(DisplayInterface& d, HistoryManager& history, int type, int pageIndex, int pageCount) {
	d.clear();
	d.text(0, 0, getHeader("Graph", pageIndex, pageCount));
	// ... Affichage simplifié pour OLED ...
	d.show();
}

void pageForecast_sh1106(DisplayInterface& d, ForecastManager& forecast, int view, int pageIndex, int pageCount) {
	d.clear();
	d.text(0, 0, getHeader("Prev.", pageIndex, pageCount));
	// ... Affichage simplifié pour OLED ...
	d.show();
}
#endif
