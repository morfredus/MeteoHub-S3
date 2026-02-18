#include "history_manager.h"
#include <LittleFS.h>
#include "../utils/logs.h"
#include <time.h>

#define HISTORY_FILE "/history/recent.dat"
#define MAX_RECENT_RECORDS 1440 // 24h à 1 point/min

void HistoryManager::begin(SdManager* sd) {
    _sd = sd;
    
    // Chargement depuis LittleFS
    loadRecent();

    // Préparation SD si disponible
    if (_sd && _sd->isAvailable()) {
        createSdStructure();
    }
}

void HistoryManager::update() {
    // Tâches de fond (archivage mensuel, nettoyage) à implémenter ici si besoin
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

    // 1. Ajout RAM (Buffer circulaire)
    _recentHistory.push_back(record);
    if (_recentHistory.size() > MAX_RECENT_RECORDS) {
        _recentHistory.erase(_recentHistory.begin());
    }

    // 2. Sauvegarde LittleFS (Append)
    saveRecent(record);

    // 3. Sauvegarde SD (Append CSV)
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
    for (const auto& r : _recentHistory) {
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

    // Lecture simple des structures binaires
    while (f.available()) {
        HistoryRecord r;
        if (f.read((uint8_t*)&r, sizeof(HistoryRecord)) == sizeof(HistoryRecord)) {
            _recentHistory.push_back(r);
        }
    }
    f.close();
    
    // Limiter la taille si le fichier est trop gros
    while (_recentHistory.size() > MAX_RECENT_RECORDS) {
        _recentHistory.erase(_recentHistory.begin());
    }
    
    LOG_INFO("History loaded: " + std::to_string(_recentHistory.size()) + " points");
}

void HistoryManager::saveRecent(const HistoryRecord& record) {
    File f = LittleFS.open(HISTORY_FILE, "a");
    if (f) {
        f.write((uint8_t*)&record, sizeof(HistoryRecord));
        f.close();
    } else {
        LOG_ERROR("Failed to append history to LittleFS");
    }
}

void HistoryManager::saveToSd(const HistoryRecord& record) {
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo, 0)) {
        LOG_WARNING("SD Save: Time not synced, cannot determine filename");
        return;
    }

    char filename[32];
    strftime(filename, sizeof(filename), "/history/%Y-%m-%d.csv", &timeinfo);
    
    // Vérifier si le dossier existe (cas de formatage ou changement de carte à chaud)
    if (!SD.exists("/history")) {
        SD.mkdir("/history");
    }

    bool fileExists = SD.exists(filename);

    File f = SD.open(filename, FILE_APPEND);
    if (f) {
        if (!fileExists) {
            LOG_INFO("Creating new daily history file on SD: " + std::string(filename));
            f.println("Timestamp,Temperature,Humidity,Pressure");
        }
        char line[64];
        snprintf(line, sizeof(line), "%ld,%.2f,%.1f,%.1f\n", record.timestamp, record.t, record.h, record.p);
        f.print(line);
        f.close();
    } else {
        LOG_WARNING("Failed to write to SD file: " + std::string(filename));
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
        if (root) {
            File file = root.openNextFile();
            while(file) {
                SD.remove(file.name());
                file = root.openNextFile();
            }
        }
    }

    LOG_INFO("History cleared");
}