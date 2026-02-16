#include "display_interface.h"
#if defined(ESP32_S3_LCD)
#include "st7789_display.h"
#include <string>
#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeSans9pt7b.h>
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
    tft->setFont(&FreeSans9pt7b); // Police large, sans accents
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
    tft->setTextSize(size);
    tft->setTextColor(color);
    tft->setFont(&FreeSans9pt7b);
    tft->setCursor(x, y);
    std::string ascii = removeAccents(s);
    tft->print(ascii.c_str());
}

void St7789Display::center(int y, const std::string& s, uint16_t color, uint8_t size) {
    tft->setTextSize(size);
    tft->setFont(&FreeSans9pt7b);
    std::string ascii = removeAccents(s);
    int16_t x1, y1;
    uint16_t w, h;
    tft->getTextBounds(ascii.c_str(), 0, y, &x1, &y1, &w, &h);
    int x = (tft->width() - w) / 2;
    text(x, y, ascii, color, size);
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

// --- placer la fonction utilitaire à la fin du fichier ---
// Fonction utilitaire pour remplacer les accents par des caractères non accentués
std::string removeAccents(const std::string& input) {
    std::string out;
    for (char c : input) {
        switch ((unsigned char)c) {
            case 0xC3: continue; // UTF-8 accent lead byte, skip (will handle next)
            case 0xA0: case 0xA1: case 0xA2: case 0xA3: case 0xA4: case 0xA5: out += 'a'; break; // àáâãäå
            case 0xA8: out += 'e'; break; // è
            case 0xA9: out += 'e'; break; // é
            case 0xAA: out += 'e'; break; // ê
            case 0xAB: out += 'e'; break; // ë
            case 0xAC: out += 'i'; break; // ì
            case 0xAD: out += 'i'; break; // í
            case 0xAE: out += 'i'; break; // î
            case 0xAF: out += 'i'; break; // ï
            case 0xB2: out += 'o'; break; // ò
            case 0xB3: out += 'o'; break; // ó
            case 0xB4: out += 'o'; break; // ô
            case 0xB5: out += 'o'; break; // õ
            case 0xB6: out += 'o'; break; // ö
            case 0xB9: out += 'u'; break; // ù
            case 0xBA: out += 'u'; break; // ú
            case 0xBB: out += 'u'; break; // û
            case 0xBC: out += 'u'; break; // ü
            case 0xA7: out += 'c'; break; // ç
            case 0x80: out += 'A'; break; // À
            case 0x89: out += 'E'; break; // É
            case 0x87: out += 'C'; break; // Ç
            default:
                if ((unsigned char)c < 0x80) out += c;
                break;
        }
    }
    return out;
}
#endif

