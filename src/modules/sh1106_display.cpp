#if defined(ESP32_S3_OLED)
#include <string>
#include "sh1106_display.h"
#include "board_config.h"
#include "config.h"

bool Sh1106Display::begin() {
    d = new SH1106Wire(0x3C, OLED_SDA, OLED_SCL);
    d->init();
    d->flipScreenVertically();
    d->setContrast(OLED_CONTRAST);
    d->clear();
    d->display();
    return true;
}

void Sh1106Display::clear() { d->clear(); }
void Sh1106Display::show() { d->display(); }

void Sh1106Display::text(int x, int y, const std::string& s) {
    d->setFont(ArialMT_Plain_10);
    d->drawString(x, y, s.c_str());
}

void Sh1106Display::center(int y, const std::string& s) {
    d->setFont(ArialMT_Plain_10);
    int w = d->getStringWidth(s.c_str());
    int x = (128 - w) / 2;
    d->drawString(x, y, s.c_str());
}

void Sh1106Display::bar(int x, int y, int w, int h, int value, int max) {
    int filled = map(value, 0, max, 0, w);
    d->drawRect(x, y, w, h);
    d->fillRect(x + 1, y + 1, filled - 2, h - 2);
}

void Sh1106Display::drawLine(int x0, int y0, int x1, int y1) {
    d->drawLine(x0, y0, x1, y1);
}
#endif
