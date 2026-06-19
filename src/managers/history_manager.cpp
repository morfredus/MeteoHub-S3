#include "history_manager.h"
#include <LittleFS.h>
#include "../utils/logs.h"
#include "../utils/cooperative_yield.h" // INCLUSION AJOUTÉE
#include <time.h>
#include <inttypes.h>
#include <Arduino.h>

#define HISTORY_FILE "/history/recent.dat"
#define MAX_RECENT_RECORDS 1440

void HistoryManager::begin(SdManager* sd) {
    _sd = sd;
    loadRecent();
    if (_sd && _sd->isAvailable()) {
        createSdStructure();
    }
}

void HistoryManager::update() {
    // Tâches de fond si nécessaire
}

void HistoryManager::add(float t, float h, float p) {
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
        LOG_WARNING("History: Time not synced, skipping record");
        return;
    }

    HistoryRecord record;
    record.timestamp = time(NULL);
    record.t = t;
    record.h = h;
    record.p = p;

    _recentHistory.push_back(record);
    if (_recentHistory.size() > MAX_RECENT_RECORDS) {
        _recentHistory.erase(_recentHistory.begin());
    }

    saveRecent(record);

    if (_sd && _sd->isAvailable()) {
        saveToSd(record);
    }
}

const std::vector<HistoryRecord>& HistoryManager::getRecentHistory() const {
    return _recentHistory;
}

Stats24h HistoryManager::getRecentStats() const {
    Stats24h stats;
    stats.count = _recentHistory.size();
    size_t stats_iteration = 0;
    for (const auto& r : _recentHistory) {
        COOPERATIVE_YIELD_EVERY(stats_iteration, 256);
        stats_iteration++;
        stats.temp.add(r.t);
        stats.hum.add(r.h);
        stats.pres.add(r.p);
    }
    return stats;
}

void HistoryManager::loadRecent() {
    if (!LittleFS.exists(HISTORY_FILE)) return;

    File f = LittleFS.open(HISTORY_FILE, "r");
    if (!f) return;

    size_t loaded_records = 0;
    while (f.available()) {
        HistoryRecord r;
        if (f.read((uint8_t*)&r, sizeof(HistoryRecord)) == sizeof(HistoryRecord)) {
            _recentHistory.push_back(r);
            loaded_records++;
            COOPERATIVE_YIELD_EVERY(loaded_records, 256);
        }
    }
    f.close();
    
    if (_recentHistory.size() > MAX_RECENT_RECORDS) {
        const size_t overflow = _recentHistory.size() - MAX_RECENT_RECORDS;
        _recentHistory.erase(_recentHistory.begin(), _recentHistory.begin() + overflow);
    }
    
    LOG_INFO("History loaded: " + std::to_string(_recentHistory.size()) + " points");
}

void HistoryManager::saveRecent(const HistoryRecord& record) {
    File f = LittleFS.open(HISTORY_FILE, "a");
    if (f) {
        f.write((uint8_t*)&record, sizeof(HistoryRecord));
        f.flush(); // Appel simple, pas de test de retour (void sur certains cores)
        f.close();
    } else {
        LOG_ERROR("Failed to append history to LittleFS");
    }
}

void HistoryManager::saveToSd(const HistoryRecord& record) {
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo, 0)) {
        LOG_WARNING("SD Save: Time not synced");
        return;
    }

    char filename[32];
    strftime(filename, sizeof(filename), "/history/%Y-%m-%d.csv", &timeinfo);

    if (!SD.exists("/history")) {
        if (!SD.mkdir("/history")) {
            LOG_WARNING("SD Save: mkdir failed for /history");
            return;
        }
        LOG_INFO("SD Save: created /history directory");
    }

    bool file_exists = SD.exists(filename);

    File f = SD.open(filename, FILE_APPEND);
    if (!f) {
        LOG_WARNING("SD Save: open failed for " + std::string(filename));
        if (_sd && _sd->ensureMounted()) {
             if (!SD.exists("/history")) SD.mkdir("/history");
             f = SD.open(filename, FILE_APPEND);
        }
        if (!f) {
            LOG_ERROR("SD Save: Critical failure, cannot open file");
            return;
        }
    }

    if (!file_exists) {
        LOG_INFO("Creating new daily history file: " + std::string(filename));
        f.println("Timestamp,Temperature,Humidity,Pressure");
    }

    char line[96];
    const long long ts = static_cast<long long>(record.timestamp);
    int written = snprintf(line, sizeof(line), "%lld,%.2f,%.1f,%.1f\n", ts, record.t, record.h, record.p);
    
    if (written > 0) {
        f.write(reinterpret_cast<const uint8_t*>(line), static_cast<size_t>(written));
        f.flush(); // Appel simple, pas de test de retour
        f.close();
    } else {
        LOG_WARNING("SD Save: Format error");
        f.close();
    }
}

void HistoryManager::createSdStructure() {
    if (!SD.exists("/history")) {
        if (SD.mkdir("/history")) {
            LOG_INFO("Created /history directory on SD card.");
        } else {
            LOG_ERROR("Failed to create /history directory on SD card.");
        }
    }
}

void HistoryManager::clearHistory() {
    _recentHistory.clear();
    LittleFS.remove(HISTORY_FILE);

    if (_sd && _sd->isAvailable()) {
        LOG_INFO("Clearing history from SD card...");
        File root = SD.open("/history");
        if (root && root.isDirectory()) {
            File file = root.openNextFile();
            while(file) {
                const char* name = file.name();
                file.close();
                SD.remove(name);
                file = root.openNextFile();
            }
            root.close();
        }
    }
    LOG_INFO("History cleared");
}

MeteoTrend HistoryManager::getTrend() const {
    MeteoTrend trend;
    if (_recentHistory.empty()) return trend;

    const time_t now = _recentHistory.back().timestamp;
    float t_now = _recentHistory.back().t;
    float h_now = _recentHistory.back().h;
    float p_now = _recentHistory.back().p;

    float t_1h = t_now, h_1h = h_now, p_1h = p_now;
    float t_24h = t_now, h_24h = h_now, p_24h = p_now;
    bool found_1h = false, found_24h = false;
    size_t trend_iteration = 0;
    
    for (auto it = _recentHistory.rbegin(); it != _recentHistory.rend(); ++it) {
        COOPERATIVE_YIELD_EVERY(trend_iteration, 256);
        trend_iteration++;

        time_t dt = now - it->timestamp;
        if (!found_1h && dt >= 3600) {
            t_1h = it->t;
            h_1h = it->h;
            p_1h = it->p;
            found_1h = true;
        }
        if (!found_24h && dt >= 86400) {
            t_24h = it->t;
            h_24h = it->h;
            p_24h = it->p;
            found_24h = true;
            break;
        }
    }

    trend.temp.delta_1h = t_now - t_1h;
    trend.temp.delta_24h = t_now - t_24h;
    trend.hum.delta_1h = h_now - h_1h;
    trend.hum.delta_24h = h_now - h_24h;
    trend.pres.delta_1h = p_now - p_1h;
    trend.pres.delta_24h = p_now - p_24h;

    auto dir = [](float d) {
        if (d > 0.2) return std::string("hausse");
        if (d < -0.2) return std::string("baisse");
        return std::string("stable");
    };
    trend.temp.direction_1h = dir(trend.temp.delta_1h);
    trend.temp.direction_24h = dir(trend.temp.delta_24h);
    trend.hum.direction_1h = dir(trend.hum.delta_1h);
    trend.hum.direction_24h = dir(trend.hum.delta_24h);
    trend.pres.direction_1h = dir(trend.pres.delta_1h);
    trend.pres.direction_24h = dir(trend.pres.delta_24h);

    return trend;
}