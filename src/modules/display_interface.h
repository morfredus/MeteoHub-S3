#pragma once
#include <string>
// Interface d'affichage générique
class DisplayInterface {
public:
    virtual ~DisplayInterface() = default;
    virtual bool begin() = 0;
    virtual void clear() = 0;
    virtual void show() = 0;
    virtual void text(int x, int y, const std::string& s) = 0;
    virtual void center(int y, const std::string& s) = 0;
    virtual void bar(int x, int y, int w, int h, int value, int max) = 0;
    virtual void drawLine(int x0, int y0, int x1, int y1) = 0;
    // 
};
