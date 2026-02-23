#include "display_interface.h"
#if defined(ESP32_S3_OLED)
#include <U8g2lib.h>
#include <Wire.h>
#include <algorithm>
#include <memory>
#include <string>
#include "oled_display.h"
#include "board_config.h"
#include "config.h"
#include "utils/logs.h"

bool OledDisplay::begin() {
    d.reset();

#if OLED_CONTROLLER == OLED_CTRL_SH1106
    d = std::make_unique<U8G2_SH1106_128X64_NONAME_F_HW_I2C>(U8G2_R0, U8X8_PIN_NONE);
    LOG_INFO("OLED backend: U8g2 SH1106 @ 0x" + std::to_string(OLED_I2C_ADDRESS));
#elif OLED_CONTROLLER == OLED_CTRL_SSD1306
    d = std::make_unique<U8G2_SSD1306_128X64_NONAME_F_HW_I2C>(U8G2_R0, U8X8_PIN_NONE);
    LOG_INFO("OLED backend: U8g2 SSD1306 @ 0x" + std::to_string(OLED_I2C_ADDRESS));
#else
    #error "Invalid OLED_CONTROLLER value in config.h"
#endif

    if (!d) {
        LOG_ERROR("OLED backend allocation failed");
        return false;
    }

    Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);
    d->setI2CAddress(static_cast<uint8_t>(OLED_I2C_ADDRESS << 1));
    d->begin();
    d->setContrast(OLED_CONTRAST);
    d->setFont(u8g2_font_6x10_tf);
    d->clearBuffer();
    d->sendBuffer();
    return true;
}

void OledDisplay::clear() {
    if (d) d->clearBuffer();
}

void OledDisplay::show() {
    if (d) d->sendBuffer();
}

void OledDisplay::text(int x, int y, const std::string& s) {
    if (!d) return;
    d->setFont(u8g2_font_6x10_tf);
    d->drawUTF8(x, y + 10, s.c_str());
}

void OledDisplay::center(int y, const std::string& s) {
    if (!d) return;
    d->setFont(u8g2_font_6x10_tf);
    int w = d->getUTF8Width(s.c_str());
    int x = (128 - w) / 2;
    if (x < 0) x = 0;
    d->drawUTF8(x, y + 10, s.c_str());
}

void OledDisplay::bar(int x, int y, int w, int h, int value, int max) {
    if (!d || max <= 0 || w <= 0 || h <= 0) return;

    int filled = map(value, 0, max, 0, w);
    filled = std::max(0, std::min(w, filled));

    d->drawFrame(x, y, w, h);
    if (filled > 2 && h > 2) {
        d->drawBox(x + 1, y + 1, filled - 2, h - 2);
    }
}

void OledDisplay::drawLine(int x0, int y0, int x1, int y1) {
    if (!d) return;
    d->drawLine(x0, y0, x1, y1);
}
#endif
