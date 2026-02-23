#pragma once

// --- WiFi Configuration ---
struct WifiCredential {
    const char* ssid;
    const char* password;
};

// Liste de vos réseaux WiFi connus
const WifiCredential WIFI_CREDENTIALS[] = {
    {"MonSSID_Maison", "MonMotDePasse"},
    {"MonSSID_Bureau", "AutreMotDePasse"}
};

const int WIFI_CREDENTIALS_COUNT = sizeof(WIFI_CREDENTIALS) / sizeof(WifiCredential);
 

// --- OpenWeatherMap Configuration ---
// Remplacez par votre clé API OpenWeatherMap (One Call API 3.0)
#define OWM_API_KEY "VOTRE_CLE_API_ICI"

// Coordonnées pour Bordeaux, FR (par défaut)
#define OWM_LAT "44.8378"
#define OWM_LON "-0.5792"

// Unités et langue
#define OWM_UNITS "metric" // "metric" pour Celsius, "imperial" pour Fahrenheit
#define OWM_LANG  "fr"     // Langue des descriptions météo

