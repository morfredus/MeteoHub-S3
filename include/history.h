#pragma once
#include "sensors.h"
#include <time.h> // For time_t

// On garde 128 points (largeur de l'écran)
#define HISTORY_SIZE 128 

class HistoryManager {
public:
    // The data structure for a single point in history
    struct Record {
        time_t timestamp;
        float t; // temperature
        float h; // humidity
        float p; // pressure
    };

    void begin();
    void update(const SensorData& data);
    
    // Copies the in-memory history to outBuffer in chronological order
    void getData(Record* outBuffer, int maxLen);
    int getCount() const;
    void clear();

private:
    // In-memory circular buffer for fast graph display
    Record buffer[HISTORY_SIZE];
    int head = 0;
    int count = 0;
    unsigned long lastUpdate = 0;
};