#include "managers/web_manager.h"
#include <ESPmDNS.h>
#include <LittleFS.h>
#include "utils/logs.h"
#include "utils/system_info.h"
// Inclusion du fichier généré automatiquement par le script Python
#include "web_pages.h"

// Si WEB_MDNS_HOSTNAME n'est pas défini dans config.h, valeur par défaut
#ifndef WEB_MDNS_HOSTNAME
#define WEB_MDNS_HOSTNAME "meteohub"
#endif

WebManager::WebManager() : _server(80) {
}

void WebManager::begin(HistoryManager& history, SdManager& sd) {
    LOG_INFO("Initialisation du WebManager...");

    _history = &history;
    _sd = &sd;
    // LittleFS n'est plus requis ici pour les pages web (géré par HistoryManager pour les données)
    // Configuration mDNS
    if (MDNS.begin(WEB_MDNS_HOSTNAME)) {
        LOG_INFO("mDNS demarre : http://" WEB_MDNS_HOSTNAME ".local");
        MDNS.addService("http", "tcp", 80);
    } else {
        LOG_ERROR("Erreur demarrage mDNS");
    }

    _setupRoutes();
    _setupApi();

    _server.begin();
    LOG_INFO("Serveur Web demarre sur le port 80");
    LOG_INFO("Gestionnaire de fichiers : http://" WEB_MDNS_HOSTNAME ".local/files.html");
    LOG_INFO("Fichiers Carte SD : http://" WEB_MDNS_HOSTNAME ".local/sd");
}

void WebManager::handle() {
    // AsyncWebServer gère les clients en background
    // Cette méthode est réservée pour des tâches de maintenance futures
}

void WebManager::_setupRoutes() {
    // Servir les fichiers depuis PROGMEM (web_pages.h)
    
    _server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(200, "text/html", web_index_html);
    });
    _server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(200, "text/css", web_style_css);
    });
    _server.on("/app.js", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(200, "application/javascript", web_app_js);
    });
    _server.on("/stats.html", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(200, "text/html", web_stats_html);
    });
    _server.on("/longterm.html", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(200, "text/html", web_longterm_html);
    });
    _server.on("/files.html", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(200, "text/html", web_files_html);
    });
    _server.on("/files.js", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(200, "application/javascript", web_files_js);
    });
    _server.on("/footer.js", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(200, "application/javascript", web_footer_js);
    });
    _server.on("/logs.html", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(200, "text/html", web_logs_html);
    });
    // Raccourci pour l'accès manuel
    _server.on("/files", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->redirect("/files.html");
    });
    // Raccourci pour l'accès SD
    _server.on("/sd", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->redirect("/files.html?fs=sd");
    });
    
    // Page Logs UI
    _server.on("/logs", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(200, "text/html", web_logs_html);
    });
    // API Logs Data
    _server.on("/api/logs", HTTP_GET, [](AsyncWebServerRequest *request) {
        AsyncResponseStream *response = request->beginResponseStream("text/plain");
        int count = getLogCount();
        for (int i = 0; i < count; i++) {
            response->print(getLog(i).c_str());
            response->print("\n");
        }
        request->send(response);
    });

    _server.onNotFound([](AsyncWebServerRequest *request) {
        request->send(404, "text/plain", "Page non trouvee");
    });
}

void WebManager::_setupApi() {
    // API : État actuel (Live)
    _server.on("/api/live", HTTP_GET, [](AsyncWebServerRequest *request) {
        // TODO: Récupérer les vraies valeurs via SensorManager
        // Exemple de structure JSON
        AsyncResponseStream *response = request->beginResponseStream("application/json");
        DynamicJsonDocument doc(512);
        
        doc["temp"] = 22.5; // Placeholder
        doc["hum"] = 45.0;  // Placeholder
        doc["pres"] = 1013.2; // Placeholder
        doc["wifi_rssi"] = WiFi.RSSI();
        doc["uptime"] = millis() / 1000;
        
        serializeJson(doc, *response);
        request->send(response);
    });

    // API : Historique
    _server.on("/api/history", HTTP_GET, [this](AsyncWebServerRequest *request) {
        // The original implementation sent one data point at a time, which could be very slow
        // and cause a watchdog timeout on large history sets.
        // This new implementation builds the JSON response in chunks to reduce network overhead
        // and prevent blocking the async_tcp task for too long.
        // UPDATE v1.116: The definitive fix is to downsample the data server-side before sending it.
        AsyncResponseStream *response = request->beginResponseStream("application/json");
        response->print("{\"data\":[");
        
        const auto& full_history = _history->getRecentHistory();
        
        // --- Server-side downsampling to prevent watchdog timeouts ---
        // The web graph doesn't need all 1440 points, so we downsample to a fixed resolution.
        // This makes the solution scalable and robust.
        const int TARGET_GRAPH_POINTS = 500;
        std::vector<HistoryRecord> records;

        if (full_history.size() <= TARGET_GRAPH_POINTS) {
            records = full_history;
        } else {
            records.reserve(TARGET_GRAPH_POINTS);
            double bucketSize = (double)full_history.size() / TARGET_GRAPH_POINTS;
            for (int i = 0; i < TARGET_GRAPH_POINTS; ++i) {
                int start_index = floor(i * bucketSize);
                int end_index = floor((i + 1) * bucketSize);
                if (end_index > (int)full_history.size()) end_index = full_history.size();

                if (start_index >= end_index) continue;

                double avg_t = 0, avg_h = 0, avg_p = 0;
                int count = 0;
                for (int j = start_index; j < end_index; ++j) {
                    avg_t += full_history[j].t;
                    avg_h += full_history[j].h;
                    avg_p += full_history[j].p;
                    count++;
                }

                if (count > 0) {
                    HistoryRecord record;
                    record.timestamp = full_history[start_index].timestamp;
                    record.t = avg_t / count;
                    record.h = avg_h / count;
                    record.p = avg_p / count;
                    records.push_back(record);
                }
            }
        }
        // --- End of downsampling ---

        String chunk;
        chunk.reserve(1024); // Reserve 1KB for the chunk

        for (size_t i = 0; i < records.size(); ++i) {
            if (i > 0) {
                chunk += ",";
            }
            char buffer[128];
            snprintf(buffer, sizeof(buffer), "{\"t\":%ld,\"temp\":%.1f,\"hum\":%.0f,\"pres\":%.1f}", (long)records[i].timestamp, records[i].t, records[i].h, records[i].p);
            chunk += buffer;

            if (chunk.length() > 800 || i == records.size() - 1) {
                response->print(chunk);
                chunk = ""; // Reset chunk
                // Give time to the network stack and watchdog
                // to prevent a timeout on very large history sets.
                delay(1); // This delay remains as a final safety net.
            }
        }
        
        response->print("]}");
        request->send(response);
    });

    // API : Statistiques
    _server.on("/api/stats", HTTP_GET, [this](AsyncWebServerRequest *request) {
        AsyncResponseStream *response = request->beginResponseStream("application/json");
        DynamicJsonDocument doc(1024);
        
        Stats24h stats = _history->getRecentStats();
        int count = stats.count;

        if (count > 0) {
            doc["temp"]["min"] = stats.temp.min;
            doc["temp"]["max"] = stats.temp.max;
            doc["temp"]["avg"] = stats.temp.avg();
            
            doc["hum"]["min"] = stats.hum.min;
            doc["hum"]["max"] = stats.hum.max;
            doc["hum"]["avg"] = stats.hum.avg();
            
            doc["pres"]["min"] = stats.pres.min;
            doc["pres"]["max"] = stats.pres.max;
            doc["pres"]["avg"] = stats.pres.avg();
        } else {
            // Valeurs par défaut si vide
            doc["temp"]["min"] = 0; doc["temp"]["max"] = 0; doc["temp"]["avg"] = 0;
            doc["hum"]["min"] = 0; doc["hum"]["max"] = 0; doc["hum"]["avg"] = 0;
            doc["pres"]["min"] = 0; doc["pres"]["max"] = 0; doc["pres"]["avg"] = 0;
        }
        doc["count"] = count;

        serializeJson(doc, *response);
        request->send(response);
    });

    // API : Système
    _server.on("/api/system", HTTP_GET, [this](AsyncWebServerRequest *request) {
        std::string system_info = getSystemInfoJson(_sd);
        request->send(200, "application/json", system_info.c_str());
    });

    // API : Liste des fichiers
    _server.on("/api/files/list", HTTP_GET, [this](AsyncWebServerRequest *request) {
        String fsName = "littlefs";
        String path = "/";
        if (request->hasParam("path")) {
            path = request->getParam("path")->value();
        }

        if (request->hasParam("fs")) {
            fsName = request->getParam("fs")->value();
        }

        fs::FS* pFs = &LittleFS;
        if (fsName == "sd") {
            if (_sd && _sd->isAvailable()) {
                pFs = &SD;
            } else {
                request->send(503, "text/plain", "SD Card not available");
                return;
            }
        }

        AsyncResponseStream *response = request->beginResponseStream("application/json");
        response->print("[");
        
        File root = pFs->open(path);
        if (!root || !root.isDirectory()) {
            root = pFs->open("/"); // Fallback root
        }

        File file = root.openNextFile();
        bool first = true;
        while(file){
            if(!first) response->print(",");
            response->print("{\"name\":\"");
            // Assurer que le nom commence par un /
            if (file.name()[0] != '/') response->print("/");
            response->print(file.name()); // file.path() sur ESP32 core > 2.0.5
            response->print("\",\"size\":");
            response->print(file.size());
            response->print(",\"isDir\":");
            response->print(file.isDirectory() ? "true" : "false");
            response->print("}");
            first = false;
            file = root.openNextFile();
        }
        response->print("]");
        request->send(response);
    });

    // API : Télécharger un fichier
    _server.on("/api/files/download", HTTP_GET, [this](AsyncWebServerRequest *request) {
        String fsName = "littlefs";
        if (request->hasParam("fs")) {
            fsName = request->getParam("fs")->value();
        }

        fs::FS* pFs = &LittleFS;
        if (fsName == "sd") {
            if (_sd && _sd->isAvailable()) {
                pFs = &SD;
            } else {
                request->send(503, "text/plain", "SD Card not available");
                return;
            }
        }

        if(request->hasParam("path")) {
            String path = request->getParam("path")->value();
            if(!path.startsWith("/")) path = "/" + path;
            if(pFs->exists(path)) {
                request->send(*pFs, path, "application/octet-stream", true);
            } else {
                request->send(404, "text/plain", "Fichier non trouve");
            }
        } else {
            request->send(400, "text/plain", "Parametre path manquant");
        }
    });

    // API : Supprimer un fichier
    _server.on("/api/files/delete", HTTP_DELETE, [this](AsyncWebServerRequest *request) {
        String fsName = "littlefs";
        if (request->hasParam("fs")) {
            fsName = request->getParam("fs")->value();
        }

        fs::FS* pFs = &LittleFS;
        if (fsName == "sd") {
            if (_sd && _sd->isAvailable()) {
                pFs = &SD;
            } else {
                request->send(503, "text/plain", "SD Card not available");
                return;
            }
        }

        if(request->hasParam("path")) {
            String path = request->getParam("path")->value();
            if(!path.startsWith("/")) path = "/" + path;
            if(pFs->exists(path)) {
                pFs->remove(path);
                request->send(200, "text/plain", "OK");
            } else {
                request->send(404, "text/plain", "Fichier non trouve");
            }
        } else {
            request->send(400, "text/plain", "Parametre path manquant");
        }
    });

    // API : Upload de fichier
    _server.on("/api/files/upload", HTTP_POST, [](AsyncWebServerRequest *request){
        request->send(200);
    }, [this](AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final){
        static File fsUploadFile;
        static fs::FS* pFsUpload;

        if(!index){
            String fsName = "littlefs";
            if (request->hasParam("fs")) {
                fsName = request->getParam("fs")->value();
            }
            pFsUpload = &LittleFS;
            if (fsName == "sd" && _sd && _sd->isAvailable()) {
                pFsUpload = &SD;
            } else if (fsName == "sd") {
                // Cannot send error here, but upload will fail
                return;
            }

            if(!filename.startsWith("/")) filename = "/" + filename;
            fsUploadFile = pFsUpload->open(filename, "w");
        }
        if(fsUploadFile){
            fsUploadFile.write(data, len);
        }
        if(final){
            if(fsUploadFile) fsUploadFile.close();
        }
    });
}