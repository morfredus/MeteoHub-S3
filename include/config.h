#pragma once

// Fréquence de rafraîchissement du dashboard
#define DASHBOARD_REFRESH_MS 1000

// Délai entre tentatives WiFi
#define WIFI_RETRY_DELAY_MS  5000

// Contraste OLED
#define OLED_CONTRAST        180

// Activer le test de ping
#define ENABLE_PING_TEST     1

// Anti-rebond pour les boutons
#define BUTTON_GUARD_MS      200

// Nom mDNS pour accès local
#define WEB_MDNS_HOSTNAME "meteohub" // Accessible via http://meteohub.local

// Configuration de l'écran LCD
#define LCD_WIDTH            240
#define LCD_HEIGHT           240 // Modifier à 240 pour l'ancien écran

// OLED driver selection (AUTO, SH1106, SSD1306)
#define OLED_DRIVER_AUTO 0
#define OLED_DRIVER_SH1106 1
#define OLED_DRIVER_SSD1306 2
#define OLED_DRIVER_MODE OLED_DRIVER_AUTO

// In AUTO mode, try SSD1306 first for broader compatibility when swapping OLED modules
#define OLED_AUTO_PREFER_SSD1306 1
