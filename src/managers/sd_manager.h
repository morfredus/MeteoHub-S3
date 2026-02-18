#pragma once
#include <FS.h>
#include <SD.h>
#include <SPI.h>
#include <string>

class SdManager {
public:
    bool begin();
    bool format();
    bool isAvailable() const;
    
    // Méthodes futures pour logs/historique
    // bool appendLog(const std::string& message);
    // bool saveHistory(const std::string& filename, const std::string& data);

private:
    bool _available = false;
};