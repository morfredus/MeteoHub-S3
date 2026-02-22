#pragma once
#if defined(ESP32_S3_OLED)
#include <Arduino.h>
#include <OLEDDisplay.h>
#include <string>
#include "display_interface.h"

class Sh1106Display : public DisplayInterface {
public:
    bool begin() override;
    void clear() override;
    void show() override;
    void text(int x, int y, const std::string& s) override;
    void center(int y, const std::string& s) override;
    void bar(int x, int y, int w, int h, int value, int max) override;
    void drawLine(int x0, int y0, int x1, int y1) override;

private:
    OLEDDisplay* d = nullptr;
};
#endif
