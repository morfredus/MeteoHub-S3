#include "system_info.h"
#include <ArduinoJson.h>
#include <LittleFS.h>
#include <SD.h>
#include <WiFi.h>
#include "config.h"
#include "managers/sd_manager.h"
#include "modules/oled_display.h"


std::string getSystemInfoJson(SdManager* sd) {
    DynamicJsonDocument doc(1024);

    doc["project_name"] = PROJECT_NAME;
    doc["project_version"] = PROJECT_VERSION;
    doc["uptime_s"] = millis() / 1000;
    doc["free_heap_b"] = ESP.getFreeHeap();
    doc["chip_model"] = ESP.getChipModel();
    doc["chip_revision"] = ESP.getChipRevision();
    doc["cpu_freq_mhz"] = ESP.getCpuFreqMHz();
    doc["flash_size_mb"] = (uint32_t)(ESP.getFlashChipSize() / (1024 * 1024));

    if (psramFound()) {
        doc["free_psram_b"] = ESP.getFreePsram();
        doc["total_psram_b"] = ESP.getPsramSize();
    }

    doc["wifi_rssi"] = WiFi.status() == WL_CONNECTED ? WiFi.RSSI() : 0;
    doc["wifi_ssid"] = WiFi.status() == WL_CONNECTED ? WiFi.SSID() : "disconnected";
    doc["ip_address"] = WiFi.localIP().toString();

    JsonObject fs_littlefs = doc.createNestedObject("littlefs");
    fs_littlefs["total_b"] = LittleFS.totalBytes();
    fs_littlefs["used_b"] = LittleFS.usedBytes();

    if (sd && sd->isAvailable()) {
        JsonObject fs_sd = doc.createNestedObject("sd_card");
        fs_sd["total_b"] = SD.totalBytes();
        fs_sd["used_b"] = SD.usedBytes();
        fs_sd["size_mb"] = (uint32_t)(SD.cardSize() / (1024 * 1024));
    } else {
        doc["sd_card"] = "not_mounted";
    }

    // Ajout diagnostic OLED
    JsonObject oled_diag = doc.createNestedObject("oled_diag");
    oled_diag["fps"] = OledDisplay::getFps();
    oled_diag["last_render_ms"] = OledDisplay::getLastRenderMs();
    oled_diag["i2c_errors"] = OledDisplay::getI2cErrorCount();

    std::string output;
    serializeJson(doc, output);
    return output;
}