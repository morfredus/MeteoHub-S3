#include "board_config.h"        // doit être en premier
#include "neopixel_status.h"
#include "config.h"
#include <Adafruit_NeoPixel.h>

static Adafruit_NeoPixel neo(1, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

void neoInit() {
    neo.begin();
    // Correction 2: réduction de luminosité pour limiter les pics de courant
    // et le bruit sur l'alimentation (impact direct sur stabilité I2C OLED).
    neo.setBrightness(NEOPIXEL_BRIGHTNESS);
    neo.show();
}

namespace {
void setNeoColorCached(uint8_t r, uint8_t g, uint8_t b) {
    static uint8_t last_r = 255;
    static uint8_t last_g = 255;
    static uint8_t last_b = 255;

    // Correction 3: éviter les écritures NeoPixel redondantes (bloquantes).
    if (r == last_r && g == last_g && b == last_b) {
        return;
    }

    last_r = r;
    last_g = g;
    last_b = b;
    neo.setPixelColor(0, neo.Color(r, g, b));
    neo.show();
}
}

void neoWifiOK() {
    setNeoColorCached(0, 150, 0); // vert
}

void neoWifiKO() {
    setNeoColorCached(150, 0, 0); // rouge
}

void neoWifiLost() {
    setNeoColorCached(150, 0, 255); // violet
}

void neoOff() {
    setNeoColorCached(0, 0, 0); // éteint
}

void neoAlertYellow() {
    setNeoColorCached(255, 255, 0); // jaune très vif
}

void neoAlertOrange() {
    setNeoColorCached(255, 100, 0); // orange soutenu
}

void neoAlertRed() {
    setNeoColorCached(255, 0, 0); // rouge pur
}
