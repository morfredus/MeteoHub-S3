// Utilitaire pour remplacer les accents par des caractères non accentués
std::string removeAccents(const std::string& input);
#pragma once
#if defined(ESP32_S3_LCD)
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include "board_config.h"
#include "display_interface.h"

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
    void text(int x, int y, const std::string& s, uint16_t color, uint8_t size);
    void center(int y, const std::string& s, uint16_t color, uint8_t size);
    void bar(int x, int y, int w, int h, int value, int max, uint16_t color);
    void drawLine(int x0, int y0, int x1, int y1, uint16_t color);

private:
    Adafruit_ST7789* tft = nullptr;
    int16_t width = 240;
    int16_t height = 240;
};
#endif
