#pragma once

#include <deque>
#include <string>
#include <vector>
#include <map>
#include <time.h>
#include <Arduino.h> // Pour String

// Structure pour un point de mesure
struct HistoryRecord {
    time_t timestamp;
    float t;
    float h;
    float p;
};

// Structure pour les statistiques d'une journée
struct DailySummary {
    float minT = 100, maxT = -100, sumT = 0;
    float minH = 100, maxH = 0, sumH = 0;
    float minP = 2000, maxP = 0, sumP = 0;
    int count = 0;

    void update(const HistoryRecord& r);
    float avgT() const { return count > 0 ? sumT / count : 0; }
    float avgH() const { return count > 0 ? sumH / count : 0; }
    float avgP() const { return count > 0 ? sumP / count : 0; }
};

// Structure pour les statistiques calculées sur 24h glissantes
struct Stats24h {
    DailySummary temp;
    DailySummary hum;
    DailySummary pres;
    int count = 0;
};

class HistoryManager {
public:
    HistoryManager();
    void begin();
    void update(); // Pour gérer les tâches périodiques
    void add(float t, float h, float p);
    void clear();

    const std::deque<HistoryRecord>& getRecentHistory() const;
    const std::map<time_t, DailySummary>& getDailySummaries() const;
    Stats24h getRecentStats() const;
    bool getArchiveData(const String& filename, std::vector<HistoryRecord>& outData);

private:
    // Constantes de configuration
    static const size_t MAX_RECENT_RECORDS = 1440; // 24h * 60 min
    static const size_t MAX_DAILY_SUMMARIES = 30;  // 30 jours
    static const int ARCHIVE_YEARS_TO_KEEP = 2;
    static const unsigned long ARCHIVE_INTERVAL_S = 15 * 60; // 15 minutes
    static const unsigned long MAINTENANCE_INTERVAL_MS = 24 * 3600 * 1000; // 1 jour

    // Données en mémoire
    std::deque<HistoryRecord> _recentHistory;
    std::map<time_t, DailySummary> _dailySummaries;

    // État pour la logique interne
    time_t _lastArchiveTimestamp = 0;
    unsigned long _lastMaintenanceCheck = 0;
    
    struct Accumulator {
        float sumT = 0, sumH = 0, sumP = 0;
        int count = 0;
        time_t startTime = 0;
    } _accumulator;

    // Méthodes de gestion de fichiers
    void _loadAllData();
    void _saveRecentRecord(const HistoryRecord& record);
    void _saveDailySummaries();
    void _appendToArchive(const HistoryRecord& record);
    void _performMaintenance();
    void _clearAllData();
};
