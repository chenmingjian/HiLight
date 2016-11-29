#ifndef __LIGHTRAW_HPP__
#define __LIGHTRAW_HPP__

#include "DataStruct.hpp"

class LightRawClass {
public:
    LightRawClass();
    void setRGB(const Color &color);
    void setRGBW(const Color &color);
    void setDirect(const Color &color);
    void setPWM(const Color &color);
    void transition();
    void transition(float scale);
    void stable() { _mode = 0; _startColor = _curColor; };
    
private:
	int 	_mode; // 0: statle; 1: transition
	Color 	_curColor;
    Color 	_startColor;
    Color   _targetColor;
};

extern LightRawClass LightRaw;

#endif

