#pragma once
#include <Arduino.h>
#include <SH1106Wire.h>

class Sh1106Display {
public:
    bool begin();
    void clear();
    void show();

    void text(int x, int y, const String& s);
    void center(int y, const String& s);
    void bar(int x, int y, int w, int h, int value, int max);
    void drawLine(int x0, int y0, int x1, int y1);


private:
    SH1106Wire* d = nullptr;
};
