
#include "board_config.h"        // doit être en premier
#include "config.h"              // pour NEOPIXEL_BRIGHTNESS_PCT
#include "neopixel_status.h"
#include <Adafruit_NeoPixel.h>

static Adafruit_NeoPixel neo(1, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

void neoInit() {
    neo.begin();
    // Calcul de la luminosité à partir du pourcentage défini dans config.h
    int brightness = (NEOPIXEL_BRIGHTNESS_PCT * 255) / 100;
    if (brightness < 0) brightness = 0;
    if (brightness > 255) brightness = 255;
    neo.setBrightness(brightness);
    neo.show();
}

void neoWifiOK() {
    neo.setPixelColor(0, neo.Color(0, 150, 0)); // vert
    neo.show();
}

void neoWifiKO() {
    neo.setPixelColor(0, neo.Color(150, 0, 0)); // rouge
    neo.show();
}

void neoWifiLost() {
    neo.setPixelColor(0, neo.Color(150, 0, 255)); // violet
    neo.show();
}

void neoOff() {
    neo.setPixelColor(0, neo.Color(0, 0, 0)); // éteint
    neo.show();
}

void neoAlertYellow() {
    // Jaune vif (RGB: 255, 220, 0)
    neo.setPixelColor(0, neo.Color(255, 220, 0));
    neo.show();
}

void neoAlertOrange() {
    // Orange franc (RGB: 255, 140, 0)
    neo.setPixelColor(0, neo.Color(255, 140, 0));
    neo.show();
}

void neoAlertRed() {
    neo.setPixelColor(0, neo.Color(255, 0, 0)); // rouge pur
    neo.show();
}

//