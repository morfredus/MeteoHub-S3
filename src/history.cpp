#include "history.h"
#include <Arduino.h>
#include <LittleFS.h>

static const char* HISTORY_FILE = "/history.dat";

void HistoryManager::begin() {
    head = 0;
    count = 0;
    lastUpdate = 0;

    if (!LittleFS.begin(true)) { // formatOnFail = true
        // This should not happen, but handle it
        return;
    }

    File file = LittleFS.open(HISTORY_FILE, "r");
    if (!file) {
        return; // No history file yet
    }

    size_t fileSize = file.size();
    if (fileSize == 0) {
        file.close();
        return;
    }

    size_t recordCount = fileSize / sizeof(Record);
    
    // Read the last HISTORY_SIZE records
    size_t recordsToRead = (recordCount < HISTORY_SIZE) ? recordCount : HISTORY_SIZE;
    
    // Seek to the correct position
    file.seek(fileSize - (recordsToRead * sizeof(Record)));

    // Read into the buffer
    count = file.read((uint8_t*)buffer, recordsToRead * sizeof(Record)) / sizeof(Record);
    head = count % HISTORY_SIZE; // Next write position
    
    file.close();
}

void HistoryManager::update(const SensorData& data) {
    // Mise a jour toutes les minutes (60000 ms)
    // Acceleration au demarrage : toutes les 5s pour les 12 premiers points
    unsigned long now_ms = millis();
    unsigned long interval = (count < 12) ? 5000 : 60000;

    if (now_ms - lastUpdate < interval && count > 0) return;
    lastUpdate = now_ms;

    time_t now_ts = time(nullptr);
    // Don't record if time is not synchronized yet
    if (now_ts < 1000000000) {
        return;
    }

    // 1. Append to file
    File file = LittleFS.open(HISTORY_FILE, "a");
    if (!file) {
        return;
    }
    Record newRecord = {now_ts, data.temperature, data.humidity, data.pressure};
    file.write((uint8_t*)&newRecord, sizeof(Record));
    file.close();

    // 2. Update in-memory buffer
    buffer[head] = newRecord;
    head = (head + 1) % HISTORY_SIZE;
    if (count < HISTORY_SIZE) count++;
}

void HistoryManager::getData(Record* outBuffer, int maxLen) {
    int itemsToCopy = (count < maxLen) ? count : maxLen;
    if (itemsToCopy == 0) return;

    int startIdx;
    if (count < HISTORY_SIZE) {
        startIdx = 0;
    } else {
        startIdx = head; // The oldest item is at the current write head
    }

    for (int i = 0; i < itemsToCopy; i++) {
        int sourceIdx = (startIdx + i) % HISTORY_SIZE;
        outBuffer[i] = buffer[sourceIdx];
    }
}

int HistoryManager::getCount() const {
    return count;
}

void HistoryManager::clear() {
    LittleFS.remove(HISTORY_FILE);
    head = 0;
    count = 0;
}