#include "managers/web_manager.h"
#include "managers/forecast_manager.h"
#include <ESPmDNS.h>
#include <LittleFS.h>
#include <cctype>
#include <string>
#include "utils/logs.h"
#include "utils/system_info.h"
// Inclusion du fichier généré automatiquement par le script Python
#include "web_pages.h"

// Si WEB_MDNS_HOSTNAME n'est pas défini dans config.h, valeur par défaut
#ifndef WEB_MDNS_HOSTNAME
#define WEB_MDNS_HOSTNAME "meteohub"
#endif

static std::string toLowerCopy(const std::string& text) {
    std::string lower = text;
    for (auto& character : lower) {
        character = static_cast<char>(std::tolower(static_cast<unsigned char>(character)));
    }
    return lower;
}

static const char* getAlertLevelLabelFr(int severity) {
    if (severity >= 3) {
        return "Rouge";
    }
    if (severity == 2) {
        return "Orange";
    }
    if (severity == 1) {
        return "Jaune";
    }
    return "Aucune";
}

static std::string translateAlertToFrench(const std::string& event) {
    const std::string lower = toLowerCopy(event);

    if (lower.find("flood") != std::string::npos || lower.find("inond") != std::string::npos || lower.find("crue") != std::string::npos) {
        return "Risque d'inondation";
    }
    if (lower.find("thunder") != std::string::npos || lower.find("orage") != std::string::npos) {
        return "Orages";
    }
    if (lower.find("rain") != std::string::npos || lower.find("pluie") != std::string::npos) {
        return "Fortes pluies";
    }
    if (lower.find("wind") != std::string::npos || lower.find("vent") != std::string::npos) {
        return "Vent fort";
    }
    if (lower.find("snow") != std::string::npos || lower.find("neige") != std::string::npos) {
        return "Neige";
    }
    if (lower.find("heat") != std::string::npos || lower.find("chaleur") != std::string::npos) {
        return "Canicule";
    }
    if (lower.find("cold") != std::string::npos || lower.find("froid") != std::string::npos || lower.find("gel") != std::string::npos) {
        return "Grand froid";
    }

    return event;
}

WebManager::WebManager() : _server(80) {
}

void WebManager::begin(HistoryManager& history, SdManager& sd, ForecastManager& forecast, SensorManager& sensors) {
    LOG_INFO("Initialisation du WebManager...");

    _history = &history;
    _sd = &sd;
    _forecast = &forecast;
    _sensors = &sensors;
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
    _server.on("/menu.js", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(200, "application/javascript", web_menu_js);
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
    _server.on("/api/live", HTTP_GET, [this](AsyncWebServerRequest *request) {
        AsyncResponseStream *response = request->beginResponseStream("application/json");
        DynamicJsonDocument doc(768);

        SensorData live_data = {0.0f, 0.0f, 0.0f, false};
        if (_sensors) {
            live_data = _sensors->read();
        }

        doc["temp"] = live_data.temperature;
        doc["hum"] = live_data.humidity;
        doc["pres"] = live_data.pressure;
        doc["sensor_valid"] = live_data.valid;
        doc["wifi_rssi"] = WiFi.RSSI();
        doc["uptime"] = millis() / 1000;

        if (_forecast) {
            doc["alert_active"] = _forecast->alert_active;
            doc["alert_severity"] = _forecast->alert.severity;
            doc["alert_sender"] = _forecast->alert.sender.c_str();
            doc["alert_event"] = _forecast->alert.event.c_str();
            doc["alert_event_fr"] = translateAlertToFrench(_forecast->alert.event).c_str();
            doc["alert_level_label_fr"] = getAlertLevelLabelFr(_forecast->alert.severity);
            doc["alert_start_unix"] = _forecast->alert.start_unix;
            doc["alert_end_unix"] = _forecast->alert.end_unix;
        } else {
            doc["alert_active"] = false;
            doc["alert_severity"] = 0;
            doc["alert_sender"] = "";
            doc["alert_event"] = "";
            doc["alert_event_fr"] = "";
            doc["alert_level_label_fr"] = "Aucune";
            doc["alert_start_unix"] = 0;
            doc["alert_end_unix"] = 0;
        }

        serializeJson(doc, *response);
        request->send(response);
    });

    _server.on("/api/alert", HTTP_GET, [this](AsyncWebServerRequest *request) {
        AsyncResponseStream *response = request->beginResponseStream("application/json");
        DynamicJsonDocument doc(1024);

        if (_forecast) {
            doc["active"] = _forecast->alert_active;
            doc["severity"] = _forecast->alert.severity;
            doc["sender"] = _forecast->alert.sender.c_str();
            doc["event"] = _forecast->alert.event.c_str();
            doc["event_fr"] = translateAlertToFrench(_forecast->alert.event).c_str();
            doc["description"] = _forecast->alert.description.c_str();
            doc["alert_level_label_fr"] = getAlertLevelLabelFr(_forecast->alert.severity);
            doc["start_unix"] = _forecast->alert.start_unix;
            doc["end_unix"] = _forecast->alert.end_unix;
        } else {
            doc["active"] = false;
            doc["severity"] = 0;
            doc["sender"] = "";
            doc["event"] = "";
            doc["event_fr"] = "";
            doc["description"] = "";
            doc["alert_level_label_fr"] = "Aucune";
            doc["start_unix"] = 0;
            doc["end_unix"] = 0;
        }

        serializeJson(doc, *response);
        request->send(response);
    });

    _server.on("/api/alert", HTTP_GET, [this](AsyncWebServerRequest *request) {
        AsyncResponseStream *response = request->beginResponseStream("application/json");
        DynamicJsonDocument doc(1024);

        if (_forecast) {
            doc["active"] = _forecast->alert_active;
            doc["severity"] = _forecast->alert.severity;
            doc["sender"] = _forecast->alert.sender.c_str();
            doc["event"] = _forecast->alert.event.c_str();
            doc["event_fr"] = translateAlertToFrench(_forecast->alert.event).c_str();
            doc["description"] = _forecast->alert.description.c_str();
        } else {
            doc["active"] = false;
            doc["severity"] = 0;
            doc["sender"] = "";
            doc["event"] = "";
            doc["event_fr"] = "";
            doc["description"] = "";
        }

        serializeJson(doc, *response);
        request->send(response);
    });

    // API : Historique
    _server.on("/api/history", HTTP_GET, [this](AsyncWebServerRequest *request) {
        // Endpoint optimisé : réduction mémoire + réponse rapide
        // Params:
        // - window=<seconds>   : fenêtre temporelle depuis le dernier point
        // - interval=<seconds> : agrégation temporelle (moyenne par bucket)
        // - points=<N>         : fallback de décimation si interval absent
        const auto& full_history = _history->getRecentHistory();

        int window_s = 0;
        if (request->hasParam("window")) {
            window_s = request->getParam("window")->value().toInt();
            if (window_s < 0) window_s = 0;
        }

        int interval_s = 0;
        if (request->hasParam("interval")) {
            interval_s = request->getParam("interval")->value().toInt();
            if (interval_s < 0) interval_s = 0;
        }

        int max_points = 0;
        if (request->hasParam("points")) {
            max_points = request->getParam("points")->value().toInt();
            if (max_points < 0) max_points = 0;
        }

        size_t start_index = 0;
        if (!full_history.empty() && window_s > 0) {
            const long latest_ts = static_cast<long>(full_history.back().timestamp);
            const long min_ts = latest_ts - static_cast<long>(window_s);

            size_t i = full_history.size();
            while (i > 0 && static_cast<long>(full_history[i - 1].timestamp) >= min_ts) {
                i--;
            }
            start_index = i;
        }

        AsyncResponseStream *response = request->beginResponseStream("application/json");
        response->print("{\"data\":[");

        bool first = true;
        const size_t available_points = full_history.size() - start_index;

        if (interval_s > 0 && available_points > 0) {
            const long latest_ts = static_cast<long>(full_history.back().timestamp);
            const long window_start_ts = (window_s > 0) ? (latest_ts - static_cast<long>(window_s)) : static_cast<long>(full_history[start_index].timestamp);

            size_t idx = start_index;
            for (long bucket_start = window_start_ts; bucket_start <= latest_ts; bucket_start += interval_s) {
                const long bucket_end = bucket_start + interval_s;
                double sum_t = 0.0;
                double sum_h = 0.0;
                double sum_p = 0.0;
                int count = 0;

                while (idx < full_history.size()) {
                    const long ts = static_cast<long>(full_history[idx].timestamp);
                    if (ts < bucket_start) {
                        idx++;
                        continue;
                    }
                    if (ts >= bucket_end) {
                        break;
                    }

                    sum_t += full_history[idx].t;
                    sum_h += full_history[idx].h;
                    sum_p += full_history[idx].p;
                    count++;
                    idx++;
                }

                if (count == 0) {
                    continue;
                }

                if (!first) {
                    response->print(",");
                }
                first = false;

                char buffer[128];
                snprintf(
                    buffer,
                    sizeof(buffer),
                    "{\"t\":%ld,\"temp\":%.1f,\"hum\":%.0f,\"pres\":%.1f}",
                    bucket_end,
                    sum_t / count,
                    sum_h / count,
                    sum_p / count
                );
                response->print(buffer);
            }
        } else {
            size_t step = 1;
            if (max_points > 0 && available_points > static_cast<size_t>(max_points)) {
                step = (available_points + static_cast<size_t>(max_points) - 1) / static_cast<size_t>(max_points);
            }

            for (size_t i = start_index; i < full_history.size(); i += step) {
                if (!first) {
                    response->print(",");
                }
                first = false;

                const auto& record = full_history[i];
                char buffer[128];
                snprintf(
                    buffer,
                    sizeof(buffer),
                    "{\"t\":%ld,\"temp\":%.1f,\"hum\":%.0f,\"pres\":%.1f}",
                    static_cast<long>(record.timestamp),
                    record.t,
                    record.h,
                    record.p
                );
                response->print(buffer);
            }
        }

        response->print("]}");
        request->send(response);
    });

    // API : Statistiques
    _server.on("/api/stats", HTTP_GET, [this](AsyncWebServerRequest *request) {
        AsyncResponseStream *response = request->beginResponseStream("application/json");
        DynamicJsonDocument doc(2048);

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
            doc["temp"]["min"] = 0; doc["temp"]["max"] = 0; doc["temp"]["avg"] = 0;
            doc["hum"]["min"] = 0; doc["hum"]["max"] = 0; doc["hum"]["avg"] = 0;
            doc["pres"]["min"] = 0; doc["pres"]["max"] = 0; doc["pres"]["avg"] = 0;
        }
        doc["count"] = count;

        // Ajout de la tendance météo
        MeteoTrend trend = _history->getTrend();
        doc["trend"]["temp"]["delta_1h"] = trend.temp.delta_1h;
        doc["trend"]["temp"]["delta_24h"] = trend.temp.delta_24h;
        doc["trend"]["temp"]["direction_1h"] = trend.temp.direction_1h.c_str();
        doc["trend"]["temp"]["direction_24h"] = trend.temp.direction_24h.c_str();
        doc["trend"]["hum"]["delta_1h"] = trend.hum.delta_1h;
        doc["trend"]["hum"]["delta_24h"] = trend.hum.delta_24h;
        doc["trend"]["hum"]["direction_1h"] = trend.hum.direction_1h.c_str();
        doc["trend"]["hum"]["direction_24h"] = trend.hum.direction_24h.c_str();
        doc["trend"]["pres"]["delta_1h"] = trend.pres.delta_1h;
        doc["trend"]["pres"]["delta_24h"] = trend.pres.delta_24h;
        doc["trend"]["pres"]["direction_1h"] = trend.pres.direction_1h.c_str();
        doc["trend"]["pres"]["direction_24h"] = trend.pres.direction_24h.c_str();

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