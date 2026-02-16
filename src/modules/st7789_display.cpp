#include "display_interface.h"
#if defined(ESP32_S3_LCD)
#include "st7789_display.h"
#include <string>
#include "board_config.h"

St7789Display::St7789Display() {}

bool St7789Display::begin() {
    // Initialisation SPI matériel
    SPIClass* spi = new SPIClass(HSPI);
    spi->begin(DISPLAY_SCK_PIN, DISPLAY_MISO_PIN, DISPLAY_MOSI_PIN, DISPLAY_CS_PIN);
    tft = new Adafruit_ST7789(spi, DISPLAY_CS_PIN, DISPLAY_DC_PIN, DISPLAY_RST_PIN);
    tft->init(width, height);
    tft->setRotation(2);
    tft->fillScreen(ST77XX_BLACK);
    tft->setTextWrap(false);
    pinMode(DISPLAY_BL_PIN, OUTPUT);
    digitalWrite(DISPLAY_BL_PIN, HIGH); // rétroéclairage ON
    return true;
}

void St7789Display::clear() {
    tft->fillScreen(ST77XX_BLACK);
}

void St7789Display::show() {
    // Pas de buffer, rien à faire
}

void St7789Display::text(int x, int y, const std::string& s, uint16_t color, uint8_t size) {
    tft->setTextColor(color);
    tft->setTextSize(size);
    tft->setCursor(x, y);
    tft->print(s.c_str());
}

void St7789Display::center(int y, const std::string& s, uint16_t color, uint8_t size) {
    int16_t x = (width - (s.length() * 6 * size)) / 2;
    text(x, y, s, color, size);
}

void St7789Display::bar(int x, int y, int w, int h, int value, int max, uint16_t color) {
    int barWidth = (w * value) / max;
    tft->drawRect(x, y, w, h, color);
    tft->fillRect(x + 1, y + 1, barWidth - 2, h - 2, color);
}

void St7789Display::drawLine(int x0, int y0, int x1, int y1, uint16_t color) {
    tft->drawLine(x0, y0, x1, y1, color);
}

// Définitions manquantes des surcharges sans couleur/size
void St7789Display::text(int x, int y, const std::string& s) {
    // Couleur et taille par défaut
    text(x, y, s, ST77XX_WHITE, 1);
}

void St7789Display::center(int y, const std::string& s) {
    center(y, s, ST77XX_WHITE, 1);
}

void St7789Display::bar(int x, int y, int w, int h, int value, int max) {
    bar(x, y, w, h, value, max, ST77XX_WHITE);
}

void St7789Display::drawLine(int x0, int y0, int x1, int y1) {
    drawLine(x0, y0, x1, y1, ST77XX_WHITE);
}
#endif

