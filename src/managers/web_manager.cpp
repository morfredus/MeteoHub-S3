#include "managers/web_manager.h"
#include <ESPmDNS.h>
#include <LittleFS.h>
#include "utils/logs.h"
// Inclusion du fichier généré automatiquement par le script Python
#include "web_pages.h"

// Si WEB_MDNS_HOSTNAME n'est pas défini dans config.h, valeur par défaut
#ifndef WEB_MDNS_HOSTNAME
#define WEB_MDNS_HOSTNAME "meteohub"
#endif

WebManager::WebManager() : _server(80), _history(nullptr) {
}

void WebManager::begin(HistoryManager& history) {
    LOG_INFO("Initialisation du WebManager...");

    _history = &history;
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
        AsyncResponseStream *response = request->beginResponseStream("application/json");
        response->print("{\"data\":[");
        
        const auto& records = _history->getRecentHistory();
        for (size_t i = 0; i < records.size(); ++i) {
            if (i > 0) response->print(",");
            // Cast explicite du timestamp en long pour correspondre à %ld (time_t est 64 bits sur ESP32)
            response->printf("{\"t\":%ld,\"temp\":%.1f,\"hum\":%.0f,\"pres\":%.1f}", (long)records[i].timestamp, records[i].t, records[i].h, records[i].p);
        }
        
        response->print("]}");
        request->send(response);
    });

    // API : Statistiques
    _server.on("/api/stats", HTTP_GET, [](AsyncWebServerRequest *request) {
        // Pour les stats, on utilise aussi le fichier history.dat (snapshot récent) ou la RAM
        // Pour simplifier, on lit history.dat qui contient le snapshot récent
        File file = LittleFS.open("/history.dat", "r");
        
        float minT = 100, maxT = -100, sumT = 0;
        float minH = 100, maxH = 0, sumH = 0;
        float minP = 2000, maxP = 0, sumP = 0;
        int count = 0;

        if (file) {
            HistoryRecord r;
            while (file.read((uint8_t*)&r, sizeof(HistoryRecord)) == sizeof(HistoryRecord)) {
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
            file.close();
        }

        AsyncResponseStream *response = request->beginResponseStream("application/json");
        DynamicJsonDocument doc(1024);
        
        if (count > 0) {
            doc["temp"]["min"] = minT;
            doc["temp"]["max"] = maxT;
            doc["temp"]["avg"] = sumT / count;
            
            doc["hum"]["min"] = minH;
            doc["hum"]["max"] = maxH;
            doc["hum"]["avg"] = sumH / count;
            
            doc["pres"]["min"] = minP;
            doc["pres"]["max"] = maxP;
            doc["pres"]["avg"] = sumP / count;
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
    _server.on("/api/system", HTTP_GET, [](AsyncWebServerRequest *request) {
        AsyncResponseStream *response = request->beginResponseStream("application/json");
        response->print("{\"version\": \"" PROJECT_VERSION "\", \"name\": \"" PROJECT_NAME "\"}");
        request->send(response);
    });

    // API : Liste des fichiers
    _server.on("/api/files/list", HTTP_GET, [](AsyncWebServerRequest *request) {
        AsyncResponseStream *response = request->beginResponseStream("application/json");
        response->print("[");
        File root = LittleFS.open("/");
        File file = root.openNextFile();
        bool first = true;
        while(file){
            if(!first) response->print(",");
            response->print("{\"name\":\"");
            response->print(file.name());
            response->print("\",\"size\":");
            response->print(file.size());
            response->print("}");
            first = false;
            file = root.openNextFile();
        }
        response->print("]");
        request->send(response);
    });

    // API : Télécharger un fichier
    _server.on("/api/files/download", HTTP_GET, [](AsyncWebServerRequest *request) {
        if(request->hasParam("path")) {
            String path = request->getParam("path")->value();
            if(!path.startsWith("/")) path = "/" + path;
            if(LittleFS.exists(path)) {
                request->send(LittleFS, path, "application/octet-stream", true);
            } else {
                request->send(404, "text/plain", "Fichier non trouve");
            }
        } else {
            request->send(400, "text/plain", "Parametre path manquant");
        }
    });

    // API : Supprimer un fichier
    _server.on("/api/files/delete", HTTP_DELETE, [](AsyncWebServerRequest *request) {
        if(request->hasParam("path")) {
            String path = request->getParam("path")->value();
            if(!path.startsWith("/")) path = "/" + path;
            if(LittleFS.exists(path)) {
                LittleFS.remove(path);
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
    }, [](AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final){
        static File fsUploadFile;
        if(!index){
            if(!filename.startsWith("/")) filename = "/" + filename;
            fsUploadFile = LittleFS.open(filename, "w");
        }
        if(fsUploadFile){
            fsUploadFile.write(data, len);
        }
        if(final){
            if(fsUploadFile) fsUploadFile.close();
        }
    });
}