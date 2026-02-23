#pragma once
#if defined(ESP32_S3_LCD)
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include "board_config.h"
#include "display_interface.h"

// Palette de couleurs
#define C_BLACK       0x0000
#define C_WHITE       0xFFFF
#define C_GREY        0x8410
#define C_DARKGREY    0x2104
#define C_BLUE        0x001F
#define C_RED         0xF800
#define C_GREEN       0x07E0
#define C_CYAN        0x07FF
#define C_MAGENTA     0xF81F
#define C_YELLOW      0xFFE0
#define C_ORANGE      0xFD20
#define C_TEAL        0x0410

class St7789Display : public DisplayInterface {
public:
    St7789Display();
    bool begin() override;
    void clear() override;
    void show() override;
    void text(int x, int y, const std::string& s) override;
    void center(int y, const std::string& s) override;
    void bar(int x, int y, int w, int h, int value, int max) override;
    void drawLine(int x0, int y0, int x1, int y1) override;

    // Fonctions spécifiques couleur/size
    void text(int x, int y, const std::string& s, uint16_t color, uint8_t size, bool alignRight = false);
    void center(int y, const std::string& s, uint16_t color, uint8_t size);
    void bar(int x, int y, int w, int h, int value, int max, uint16_t color);
    void drawLine(int x0, int y0, int x1, int y1, uint16_t color);
    void drawRect(int x, int y, int w, int h, uint16_t color);
    void fillRoundRect(int x, int y, int w, int h, int r, uint16_t color);
    void fillScreen(uint16_t color);
    void drawPixel(int x, int y, uint16_t color);
    
    // Helpers UI "Pro"
    void drawHeader(const std::string& title, const std::string& rightText);
    void drawCard(int x, int y, int w, int h, uint16_t color);
    void drawLabelValue(int x, int y, const std::string& label, const std::string& value, uint16_t labelColor, uint16_t valueColor);
    int width() const { return 240; }
    int height() const { return 240; }

private:
    Adafruit_ST7789* tft = nullptr;
    GFXcanvas16* canvas = nullptr;
    int16_t _width = 240;
    int16_t _height = 240;
};
#endif
