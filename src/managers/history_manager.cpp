#include "managers/history_manager.h"
#include <LittleFS.h>
#include "utils/logs.h"

HistoryManager::HistoryManager() {}

void HistoryManager::begin() {
    LOG_INFO("Initialisation HistoryManager...");
    
    if (!LittleFS.begin(true)) {
        LOG_ERROR("Echec montage LittleFS");
        return;
    }

    // Chargement de l'historique récent depuis le fichier de sauvegarde (crash recovery)
    if (LittleFS.exists("/history.dat")) {
        File f = LittleFS.open("/history.dat", "r");
        if (f) {
            // Optimisation : ne lire que les derniers enregistrements si le fichier est trop gros
            size_t fileSize = f.size();
            size_t recordSize = sizeof(HistoryRecord);
            size_t totalRecords = fileSize / recordSize;
            
            if (totalRecords > _maxRecentRecords) {
                f.seek((totalRecords - _maxRecentRecords) * recordSize);
            }

            HistoryRecord r;
            while (f.read((uint8_t*)&r, recordSize) == recordSize) {
                _recentHistory.push_back(r);
                // Garder seulement les 1440 derniers points si le fichier est corrompu/trop gros
                if (_recentHistory.size() > _maxRecentRecords) {
                    _recentHistory.pop_front();
                }
                yield(); // Important : évite le déclenchement du Watchdog sur gros fichiers
            }
            f.close();
            LOG_INFO("Historique restaure : " + std::to_string(_recentHistory.size()) + " points");
        }
    }
}

void HistoryManager::add(float t, float h, float p) {
    time_t now;
    time(&now);
    struct tm timeinfo;
    localtime_r(&now, &timeinfo);

    HistoryRecord r = { now, t, h, p };

    // 1. Ajout au buffer RAM (Haute Résolution)
    _recentHistory.push_back(r);
    if (_recentHistory.size() > _maxRecentRecords) {
        _recentHistory.pop_front();
    }

    // Sauvegarde périodique du buffer RAM (ex: toutes les heures ou si changement important)
    // Pour limiter l'usure flash, on ne sauvegarde le fichier "current" que toutes les 10 min
    if (timeinfo.tm_min % 10 == 0 && timeinfo.tm_sec < 2) { // < 2 pour éviter écritures multiples dans la seconde
        _saveRecentToFlash();
    }

    // 2. Gestion de l'archivage (Décimation)
    _accumulator.sumT += t;
    _accumulator.sumH += h;
    _accumulator.sumP += p;
    _accumulator.count++;

    // Archivage toutes les 15 minutes (00, 15, 30, 45)
    if (timeinfo.tm_min % 15 == 0 && _accumulator.count > 0) {
        HistoryRecord avgRecord;
        avgRecord.timestamp = now;
        avgRecord.t = _accumulator.sumT / _accumulator.count;
        avgRecord.h = _accumulator.sumH / _accumulator.count;
        avgRecord.p = _accumulator.sumP / _accumulator.count;

        _appendToArchive(avgRecord);

        // Reset accumulateur
        _accumulator.sumT = 0;
        _accumulator.sumH = 0;
        _accumulator.sumP = 0;
        _accumulator.count = 0;
    }
}

void HistoryManager::clear() {
    _recentHistory.clear();
    LittleFS.remove("/history.dat");
    _accumulator.sumT = 0;
    _accumulator.sumH = 0;
    _accumulator.sumP = 0;
    _accumulator.count = 0;
    LOG_INFO("Historique efface");
}

void HistoryManager::_saveRecentToFlash() {
    // Écriture complète du buffer RAM vers history.dat (snapshot)
    File f = LittleFS.open("/history.dat", "w");
    if (f) {
        for (const auto& r : _recentHistory) {
            f.write((uint8_t*)&r, sizeof(HistoryRecord));
        }
        f.close();
        LOG_DEBUG("Snapshot historique sauvegarde");
    }
}

void HistoryManager::_appendToArchive(const HistoryRecord& record) {
    // Nom du fichier basé sur le mois : /YYYY-MM.dat
    struct tm timeinfo;
    localtime_r(&record.timestamp, &timeinfo);
    
    char filename[16];
    snprintf(filename, sizeof(filename), "/%04d-%02d.dat", timeinfo.tm_year + 1900, timeinfo.tm_mon + 1);

    File f = LittleFS.open(filename, "a");
    if (f) {
        f.write((uint8_t*)&record, sizeof(HistoryRecord));
        f.close();
        LOG_INFO("Archivage point dans " + std::string(filename));
    } else {
        LOG_ERROR("Erreur ouverture archive " + std::string(filename));
    }
}

const std::deque<HistoryRecord>& HistoryManager::getRecentHistory() const {
    return _recentHistory;
}

bool HistoryManager::getArchiveData(const String& filename, std::vector<HistoryRecord>& outData) {
    if (!LittleFS.exists(filename)) return false;

    File f = LittleFS.open(filename, "r");
    if (!f) return false;

    HistoryRecord r;
    while (f.read((uint8_t*)&r, sizeof(HistoryRecord)) == sizeof(HistoryRecord)) {
        outData.push_back(r);
    }
    f.close();
    return true;
}