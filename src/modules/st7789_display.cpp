#include "display_interface.h"
#if defined(ESP32_S3_LCD)
#include "st7789_display.h"
#include <string>
#include <Fonts/FreeSans9pt7b.h>
#include "board_config.h"
#include "config.h"

// Forward declaration helper
std::string removeAccents(const std::string& input);
 
St7789Display::St7789Display() {
    _width = LCD_WIDTH;
    _height = LCD_HEIGHT;
}

bool St7789Display::begin() {
    // Initialisation SPI matériel
    SPIClass* spi = new SPIClass(HSPI);
    spi->begin(DISPLAY_SCK_PIN, DISPLAY_MISO_PIN, DISPLAY_MOSI_PIN, DISPLAY_CS_PIN);
    tft = new Adafruit_ST7789(spi, DISPLAY_CS_PIN, DISPLAY_DC_PIN, DISPLAY_RST_PIN);
    tft->init(_width, _height);
    tft->setRotation(2);
    
    // Double buffering pour éviter le scintillement
    canvas = new GFXcanvas16(_width, _height);
    canvas->setFont(&FreeSans9pt7b);
    canvas->setTextWrap(false);
    canvas->fillScreen(C_BLACK);

    pinMode(DISPLAY_BL_PIN, OUTPUT);
    digitalWrite(DISPLAY_BL_PIN, HIGH); // rétroéclairage ON
    return true;
}

void St7789Display::clear() {
    canvas->fillScreen(C_BLACK);
}

void St7789Display::show() {
    tft->drawRGBBitmap(0, 0, canvas->getBuffer(), _width, _height);
}

void St7789Display::text(int x, int y, const std::string& s, uint16_t color, uint8_t size, bool alignRight) {
    canvas->setTextSize(size);
    canvas->setTextColor(color);
    canvas->setFont(&FreeSans9pt7b);
    std::string ascii = removeAccents(s);
    
    if (alignRight) {
        int16_t x1, y1;
        uint16_t w, h;
        canvas->getTextBounds(ascii.c_str(), 0, 0, &x1, &y1, &w, &h);
        canvas->setCursor(x - w, y);
    } else {
        canvas->setCursor(x, y);
    }
    canvas->print(ascii.c_str());
}

void St7789Display::center(int y, const std::string& s, uint16_t color, uint8_t size) {
    canvas->setTextSize(size);
    canvas->setFont(&FreeSans9pt7b);
    std::string ascii = removeAccents(s);
    int16_t x1, y1;
    uint16_t w, h;
    canvas->getTextBounds(ascii.c_str(), 0, y, &x1, &y1, &w, &h);
    int x = (_width - w) / 2;
    text(x, y, ascii, color, size);
}

void St7789Display::bar(int x, int y, int w, int h, int value, int max, uint16_t color) {
    int barWidth = (w * value) / max;
    canvas->drawRect(x, y, w, h, color);
    canvas->fillRect(x + 1, y + 1, barWidth - 2, h - 2, color);
}

void St7789Display::drawLine(int x0, int y0, int x1, int y1, uint16_t color) {
    canvas->drawLine(x0, y0, x1, y1, color);
}

void St7789Display::drawRect(int x, int y, int w, int h, uint16_t color) {
    canvas->drawRect(x, y, w, h, color);
}

void St7789Display::fillRoundRect(int x, int y, int w, int h, int r, uint16_t color) {
    canvas->fillRoundRect(x, y, w, h, r, color);
}

void St7789Display::fillScreen(uint16_t color) {
    canvas->fillScreen(color);
}

void St7789Display::drawPixel(int x, int y, uint16_t color) {
    canvas->drawPixel(x, y, color);
}

// Helpers UI
void St7789Display::drawHeader(const std::string& title, const std::string& rightText) {
    // Fond de l'en-tête
    canvas->fillRect(0, 0, _width, 32, C_DARKGREY);
    canvas->drawFastHLine(0, 32, _width, C_GREY);
    
    // Titre à gauche
    text(5, 22, title, C_CYAN, 1);
    
    // Info à droite (Heure ou Page)
    if (!rightText.empty()) {
        text(_width - 5, 22, rightText, C_WHITE, 1, true);
    }
}

void St7789Display::drawCard(int x, int y, int w, int h, uint16_t color) {
    canvas->drawRoundRect(x, y, w, h, 4, color);
    canvas->fillRoundRect(x, y, w, h, 4, C_DARKGREY); // Fond légèrement plus clair que le noir
}

void St7789Display::drawLabelValue(int x, int y, const std::string& label, const std::string& value, uint16_t labelColor, uint16_t valueColor) {
    text(x, y, label, labelColor, 1);
    text(x + 100, y, value, valueColor, 1); // Alignement simple colonne
}

// Définitions manquantes des surcharges sans couleur/size
void St7789Display::text(int x, int y, const std::string& s) {
    // Couleur et taille par défaut
    text(x, y, s, C_WHITE, 1);
}

void St7789Display::center(int y, const std::string& s) {
    center(y, s, C_WHITE, 1);
}

void St7789Display::bar(int x, int y, int w, int h, int value, int max) {
    bar(x, y, w, h, value, max, C_WHITE);
}

void St7789Display::drawLine(int x0, int y0, int x1, int y1) {
    drawLine(x0, y0, x1, y1, C_WHITE);
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
            case 0x82: out += 'A'; break; // Â
            case 0x88: out += 'E'; break; // È
            case 0x89: out += 'E'; break; // É
            case 0x8A: out += 'E'; break; // Ê
            case 0x8B: out += 'E'; break; // Ë
            case 0x8E: out += 'I'; break; // Î
            case 0x8F: out += 'I'; break; // Ï
            case 0x94: out += 'O'; break; // Ô
            case 0x99: out += 'U'; break; // Ù
            case 0x9B: out += 'U'; break; // Û
            case 0x87: out += 'C'; break; // Ç
            default:
                if ((unsigned char)c < 0x80) out += c;
                break;
        }
    }
    return out;
}
#endif
