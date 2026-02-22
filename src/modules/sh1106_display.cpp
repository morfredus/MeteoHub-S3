#include "display_interface.h"
#if defined(ESP32_S3_OLED)
#include <Wire.h>
#include <string>
#include <SSD1306Wire.h>
#include <SH1106Wire.h>

#include "sh1106_display.h"
#include "board_config.h"
#include "config.h"
#include "utils/logs.h"

bool Sh1106Display::isDevicePresent(uint8_t address) const {
    if (address == 0) return false;
    Wire.beginTransmission(address);
    return Wire.endTransmission() == 0;
}

uint8_t Sh1106Display::detectOledAddress() const {
    const uint8_t candidates[] = {0x3C, 0x3D};

    for (uint8_t addr : candidates) {
        if (isDevicePresent(addr)) {
            return addr;
        }
    }

    return 0;
}

bool Sh1106Display::beginWithDriver(uint8_t address, int driver_mode) {
    if (d) {
        delete d;
        d = nullptr;
    }

    active_address = address;

    if (driver_mode == OLED_DRIVER_SH1106) {
        d = new SH1106Wire(address, I2C_SDA_PIN, I2C_SCL_PIN);
        active_driver = OLED_DRIVER_ACTIVE_SH1106;
        LOG_INFO("OLED driver selected: SH1106");
    } else {
        d = new SSD1306Wire(address, I2C_SDA_PIN, I2C_SCL_PIN);
        active_driver = OLED_DRIVER_ACTIVE_SSD1306;
        LOG_INFO("OLED driver selected: SSD1306");
    }

    d->init();
    d->flipScreenVertically();
    d->setContrast(OLED_CONTRAST);
    d->resetDisplay();
    delay(8);
    d->clear();
    d->display();
    delay(8);
    d->clear();
    d->display();

    device_was_missing = false;
    return true;
}

bool Sh1106Display::begin() {
    Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);
    const uint8_t oled_address = detectOledAddress();
    if (oled_address == 0) {
        LOG_ERROR("No OLED detected on I2C (0x3C/0x3D)");
        active_driver = OLED_DRIVER_UNKNOWN;
        return false;
    }

    LOG_INFO("OLED detected at I2C address: 0x" + std::to_string(oled_address));

#if OLED_DRIVER_MODE == OLED_DRIVER_SH1106
    return beginWithDriver(oled_address, OLED_DRIVER_SH1106);
#elif OLED_DRIVER_MODE == OLED_DRIVER_SSD1306
    return beginWithDriver(oled_address, OLED_DRIVER_SSD1306);
#else
#if OLED_AUTO_PREFER_SSD1306
    if (beginWithDriver(oled_address, OLED_DRIVER_SSD1306)) {
        return true;
    }
    return beginWithDriver(oled_address, OLED_DRIVER_SH1106);
#else
    if (beginWithDriver(oled_address, OLED_DRIVER_SH1106)) {
        return true;
    }
    return beginWithDriver(oled_address, OLED_DRIVER_SSD1306);
#endif
#endif
}

void Sh1106Display::clear() { if (d) d->clear(); }
void Sh1106Display::show() {
    if (!d) return;

    const bool present = isDevicePresent(active_address);
    if (!present) {
        device_was_missing = true;
        return;
    }

    if (device_was_missing) {
        LOG_WARNING("OLED hot-plug detected, reinitializing display driver");
        beginWithDriver(active_address, active_driver == OLED_DRIVER_ACTIVE_SSD1306 ? OLED_DRIVER_SSD1306 : OLED_DRIVER_SH1106);
    }

    d->display();
}

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
