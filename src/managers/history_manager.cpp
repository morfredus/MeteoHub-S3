#include "managers/history_manager.h"
#include <LittleFS.h>
#include "utils/logs.h"

const char* RECENT_HISTORY_PATH = "/history/recent.dat";
const char* DAILY_SUMMARIES_PATH = "/history/summaries.dat";
const char* ARCHIVE_DIR = "/archive";

// --- Implémentation DailySummary ---
void DailySummary::update(const HistoryRecord& r) {
    if (r.t < minT) minT = r.t;
    if (r.t > maxT) maxT = r.t;
    sumT += r.t;

    if (r.h < minH) minH = r.h;
    if (r.h > maxH) maxH = r.h;
    sumH += r.h;

    if (r.p < minP) minP = r.p;
    if (r.p > maxP) maxP = r.p;
    sumP += r.p;

    count++;
}

// --- Implémentation HistoryManager ---
HistoryManager::HistoryManager() {}

void HistoryManager::begin() {
    LOG_INFO("Initialisation HistoryManager...");
    if (!LittleFS.begin(true)) {
        LOG_ERROR("Echec montage LittleFS");
        return;
    }
    // Créer les répertoires si nécessaire
    if (!LittleFS.exists("/history")) LittleFS.mkdir("/history");
    if (!LittleFS.exists(ARCHIVE_DIR)) LittleFS.mkdir(ARCHIVE_DIR);

    _loadAllData();
    _lastMaintenanceCheck = millis();
}

void HistoryManager::update() {
    if (millis() - _lastMaintenanceCheck > MAINTENANCE_INTERVAL_MS) {
        _performMaintenance();
        _lastMaintenanceCheck = millis();
    }
}

void HistoryManager::add(float t, float h, float p) {
    time_t now;
    time(&now);

    // Amélioration de la robustesse : Ne pas enregistrer de point si l'heure n'est pas synchronisée.
    // Une date valide sera toujours supérieure à l'an 2020 (timestamp > 1577836800).
    if (now < 1577836800) {
        return; // L'heure NTP n'est pas encore synchronisée, on ignore ce point.
    }

    HistoryRecord r = { now, t, h, p };

    // 1. Ajout au buffer RAM et au fichier "recent"
    _recentHistory.push_back(r);
    if (_recentHistory.size() > MAX_RECENT_RECORDS) {
        _recentHistory.pop_front();
    }
    _saveRecentRecord(r);

    // 2. Mise à jour des résumés quotidiens
    struct tm timeinfo;
    localtime_r(&now, &timeinfo);
    timeinfo.tm_hour = 0; timeinfo.tm_min = 0; timeinfo.tm_sec = 0;
    time_t dayStart = mktime(&timeinfo);

    _dailySummaries[dayStart].update(r);

    // 3. Gestion de l'archivage (Décimation)
    if (_accumulator.count == 0) {
        _accumulator.startTime = now;
    }
    _accumulator.sumT += t;
    _accumulator.sumH += h;
    _accumulator.sumP += p;
    _accumulator.count++;

    if (now - _accumulator.startTime >= ARCHIVE_INTERVAL_S && _accumulator.count > 0) {
        HistoryRecord avgRecord;
        avgRecord.timestamp = now;
        avgRecord.t = _accumulator.sumT / _accumulator.count;
        avgRecord.h = _accumulator.sumH / _accumulator.count;
        avgRecord.p = _accumulator.sumP / _accumulator.count;

        _appendToArchive(avgRecord);

        // Reset accumulateur
        _accumulator = {};
    }
}

void HistoryManager::clear() {
    _clearAllData();
    LOG_INFO("Historique complet efface");
}

void HistoryManager::_loadAllData() {
    // 1. Charger l'historique récent
    if (LittleFS.exists(RECENT_HISTORY_PATH)) {
        File f = LittleFS.open(RECENT_HISTORY_PATH, "r");
        if (f) {
            size_t recordSize = sizeof(HistoryRecord);
            size_t totalRecords = f.size() / recordSize;
            if (totalRecords > MAX_RECENT_RECORDS) {
                f.seek((totalRecords - MAX_RECENT_RECORDS) * recordSize);
            }
            HistoryRecord r;
            while (f.read((uint8_t*)&r, recordSize) == recordSize) {
                _recentHistory.push_back(r);
            }
            f.close();
            LOG_INFO("Historique recent restaure: " + std::to_string(_recentHistory.size()) + " points");
        }
    }

    // 2. Charger les résumés quotidiens
    if (LittleFS.exists(DAILY_SUMMARIES_PATH)) {
        File f = LittleFS.open(DAILY_SUMMARIES_PATH, "r");
        if (f) {
            time_t key;
            DailySummary summary;
            while (f.read((uint8_t*)&key, sizeof(key)) == sizeof(key) &&
                   f.read((uint8_t*)&summary, sizeof(summary)) == sizeof(summary)) {
                _dailySummaries[key] = summary;
            }
            f.close();
            LOG_INFO("Resumes quotidiens restaures: " + std::to_string(_dailySummaries.size()) + " jours");
        }
    }
}

void HistoryManager::_saveRecentRecord(const HistoryRecord& record) {
    File f = LittleFS.open(RECENT_HISTORY_PATH, "a");
    if (f) {
        f.write((uint8_t*)&record, sizeof(HistoryRecord));
        f.close();
    } else {
        LOG_ERROR("Erreur ecriture historique recent");
    }
}

void HistoryManager::_saveDailySummaries() {
    File f = LittleFS.open(DAILY_SUMMARIES_PATH, "w");
    if (f) {
        for (const auto& pair : _dailySummaries) {
            f.write((uint8_t*)&pair.first, sizeof(pair.first));
            f.write((uint8_t*)&pair.second, sizeof(pair.second));
        }
        f.close();
        LOG_INFO("Resumes quotidiens sauvegardes");
    } else {
        LOG_ERROR("Erreur sauvegarde resumes quotidiens");
    }
}

void HistoryManager::_appendToArchive(const HistoryRecord& record) {
    struct tm timeinfo;
    localtime_r(&record.timestamp, &timeinfo);
    
    char filename[32];
    snprintf(filename, sizeof(filename), "%s/%04d-%02d.dat", ARCHIVE_DIR, timeinfo.tm_year + 1900, timeinfo.tm_mon + 1);

    File f = LittleFS.open(filename, "a");
    if (f) {
        f.write((uint8_t*)&record, sizeof(HistoryRecord));
        f.close();
        LOG_INFO("Archivage point dans " + std::string(filename));
    } else {
        LOG_ERROR("Erreur ouverture archive " + std::string(filename));
    }
}

void HistoryManager::_performMaintenance() {
    LOG_INFO("Maintenance de l'historique...");

    // 1. Nettoyer le buffer et le fichier de l'historique récent
    while (_recentHistory.size() > MAX_RECENT_RECORDS) {
        _recentHistory.pop_front();
    }
    File recentFile = LittleFS.open(RECENT_HISTORY_PATH, "w");
    if (recentFile) {
        for (const auto& r : _recentHistory) {
            recentFile.write((uint8_t*)&r, sizeof(HistoryRecord));
        }
        recentFile.close();
        LOG_INFO("Fichier historique recent nettoye.");
    }

    // 2. Nettoyer les résumés quotidiens de plus de 30 jours et sauvegarder
    time_t now;
    time(&now);
    while (_dailySummaries.size() > MAX_DAILY_SUMMARIES) {
        auto it = _dailySummaries.begin();
        if (now - it->first > (MAX_DAILY_SUMMARIES * 24 * 3600)) {
            _dailySummaries.erase(it);
        } else {
            break; // Le plus ancien est encore valide
        }
    }
    _saveDailySummaries();

    // 3. Supprimer les fichiers d'archive de plus de 2 ans
    File root = LittleFS.open(ARCHIVE_DIR);
    File file = root.openNextFile();
    struct tm timeinfo;
    localtime_r(&now, &timeinfo);
    int currentYear = timeinfo.tm_year + 1900;
    int currentMonth = timeinfo.tm_mon + 1;

    while(file){
        int fileYear, fileMonth;
        // CORRECTIF : Le nom du fichier inclut le répertoire. Le format de scan doit correspondre.
        // ex: sscanf("/archive/2024-05.dat", "/archive/%d-%d.dat", ...)
        char scan_format[40];
        snprintf(scan_format, sizeof(scan_format), "%s/%%d-%%d.dat", ARCHIVE_DIR);
        if(sscanf(file.name(), scan_format, &fileYear, &fileMonth) == 2) {
            if ((currentYear - fileYear) * 12 + (currentMonth - fileMonth) >= (ARCHIVE_YEARS_TO_KEEP * 12)) {
                LOG_INFO("Suppression vieille archive: " + std::string(file.name()));
                LittleFS.remove(file.name()); // CORRECTIF : file.name() contient déjà le chemin complet
            }
        }
        file = root.openNextFile();
    }
    LOG_INFO("Maintenance terminee.");
}

void HistoryManager::_clearAllData() {
    _recentHistory.clear();
    _dailySummaries.clear();
    _accumulator = {};
    LittleFS.remove(RECENT_HISTORY_PATH);
    LittleFS.remove(DAILY_SUMMARIES_PATH);
    File root = LittleFS.open(ARCHIVE_DIR);
    File file = root.openNextFile();
    while(file){
        LittleFS.remove(file.name()); // CORRECTIF : file.name() contient déjà le chemin complet
        file = root.openNextFile();
    }
}

const std::deque<HistoryRecord>& HistoryManager::getRecentHistory() const {
    return _recentHistory;
}

const std::map<time_t, DailySummary>& HistoryManager::getDailySummaries() const {
    return _dailySummaries;
}

Stats24h HistoryManager::getRecentStats() const {
    Stats24h stats;
    for (const auto& r : _recentHistory) {
        stats.temp.update(r);
        stats.hum.update(r);
        stats.pres.update(r);
    }
    stats.count = _recentHistory.size();
    return stats;
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