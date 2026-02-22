#include "display_interface.h"
#if defined(ESP32_S3_OLED)
#include <string>
#include <SSD1306Wire.h>
#include <SH1106Wire.h>

#include "sh1106_display.h"
#include "board_config.h"
#include "config.h"
#include "utils/logs.h"

bool Sh1106Display::begin() {
    if (d) {
        delete d;
        d = nullptr;
    }

#if OLED_CONTROLLER == OLED_CTRL_SH1106
    d = new SH1106Wire(OLED_I2C_ADDRESS, I2C_SDA_PIN, I2C_SCL_PIN);
    LOG_INFO("OLED controller configured: SH1106 @ 0x" + std::to_string(OLED_I2C_ADDRESS));
#elif OLED_CONTROLLER == OLED_CTRL_SSD1306
    d = new SSD1306Wire(OLED_I2C_ADDRESS, I2C_SDA_PIN, I2C_SCL_PIN);
    LOG_INFO("OLED controller configured: SSD1306 @ 0x" + std::to_string(OLED_I2C_ADDRESS));
#else
    #error "Invalid OLED_CONTROLLER value in config.h"
#endif

    d->init();
    d->flipScreenVertically();
    d->setContrast(OLED_CONTRAST);
    d->resetDisplay();
    d->clear();
    d->display();
    return true;
}

void Sh1106Display::clear() { if (d) d->clear(); }
void Sh1106Display::show() { if (d) d->display(); }

void Sh1106Display::text(int x, int y, const std::string& s) {
    if (!d) return;
    d->setFont(ArialMT_Plain_10);
    d->drawString(x, y, s.c_str());
}

void Sh1106Display::center(int y, const std::string& s) {
    if (!d) return;
    d->setFont(ArialMT_Plain_10);
    int w = d->getStringWidth(s.c_str());
    int x = (128 - w) / 2;
    d->drawString(x, y, s.c_str());
}

void Sh1106Display::bar(int x, int y, int w, int h, int value, int max) {
    if (!d) return;
    int filled = map(value, 0, max, 0, w);
    d->drawRect(x, y, w, h);
    d->fillRect(x + 1, y + 1, filled - 2, h - 2);
}

void Sh1106Display::drawLine(int x0, int y0, int x1, int y1) {
    if (!d) return;
    d->drawLine(x0, y0, x1, y1);
}
#endif
