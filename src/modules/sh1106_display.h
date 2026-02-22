#pragma once
#if defined(ESP32_S3_OLED)
#include <Arduino.h>
#include <OLEDDisplay.h>
#include <string>
#include "display_interface.h"

enum OledDriverType {
    OLED_DRIVER_UNKNOWN = 0,
    OLED_DRIVER_ACTIVE_SH1106 = 1,
    OLED_DRIVER_ACTIVE_SSD1306 = 2
};

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
    OledDriverType active_driver = OLED_DRIVER_UNKNOWN;
    uint8_t active_address = 0;
    bool device_was_missing = false;
    bool beginWithDriver(uint8_t address, int driver_mode);
    uint8_t detectOledAddress() const;
    bool isDevicePresent(uint8_t address) const;
};
#endif
