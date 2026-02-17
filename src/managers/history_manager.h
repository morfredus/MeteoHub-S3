#ifndef HISTORY_MANAGER_H
#define HISTORY_MANAGER_H

#include <Arduino.h>
#include <vector>
#include <deque>
#include <time.h>

// Structure de données pour un point d'historique
struct HistoryRecord {
    time_t timestamp;
    float t; // Température
    float h; // Humidité
    float p; // Pression
};

class HistoryManager {
public:
    HistoryManager();
    void begin();
    void add(float t, float h, float p);
    void clear();
    
    // Accès aux données haute résolution (RAM)
    const std::deque<HistoryRecord>& getRecentHistory() const;
    
    // Récupération des archives (lecture fichier)
    // Retourne true si le fichier existe et a été ouvert
    bool getArchiveData(const String& filename, std::vector<HistoryRecord>& outData);

private:
    // Buffer circulaire en RAM pour les dernières 24h (1 point/min = 1440 points)
    std::deque<HistoryRecord> _recentHistory;
    const size_t _maxRecentRecords = 1440; 

    // Accumulateur pour la décimation (moyenne 15 min)
    struct {
        float sumT = 0;
        float sumH = 0;
        float sumP = 0;
        int count = 0;
    } _accumulator;

    void _saveRecentToFlash();
    void _appendToArchive(const HistoryRecord& record);
};

#endif // HISTORY_MANAGER_H