#pragma once
#if defined(ESP32_S3_OLED) || defined(ESP32_DEV_MODULE_OLED)
#include <Arduino.h>
#include <U8g2lib.h>
#include <memory>
#include <string>
#include "display_interface.h"
 
class OledDisplay : public DisplayInterface {
public:
    bool begin() override;
    void clear() override;
    void show() override;
    void text(int x, int y, const std::string& s) override;
    void center(int y, const std::string& s) override;
    void bar(int x, int y, int w, int h, int value, int max) override;
    void drawLine(int x0, int y0, int x1, int y1) override;

    // Diagnostic statique
    static float getFps();
    static uint32_t getLastRenderMs();
    static uint32_t getI2cErrorCount();

    // Pour mise à jour
    static void notifyRender(uint32_t durationMs);
    static void notifyI2cError();

private:
    std::unique_ptr<U8G2> d;
    static float fps;
    static uint32_t last_render_ms;
    static uint32_t i2c_error_count;
    static uint32_t last_render_time;
};

// Alias explicite pour refléter le backend OLED U8g2 actuel
using OledDisplayAlias = OledDisplay;
#endif
