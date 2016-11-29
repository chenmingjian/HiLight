#include <Arduino.h>
#include "LightRaw.hpp"


static constexpr int led_r = 16;
static constexpr int led_g = 5;
static constexpr int led_b = 4;
static constexpr int led_w = 2;

LightRawClass LightRaw;

static inline uint8_t tou8(int value) {
    if (value > 255) return 255;
    else if (value < 0) return 0;
    else return (uint8_t)value;
}

LightRawClass::LightRawClass() {
	pinMode(led_r, OUTPUT);
	pinMode(led_g, OUTPUT);
	pinMode(led_b, OUTPUT);
	pinMode(led_w, OUTPUT);
    analogWriteFreq(300);
    analogWriteRange(255);
    stable();
    setDirect(Color());
}

void LightRawClass::setPWM(const Color &color) {
    analogWrite(led_r, color.r);
    analogWrite(led_g, color.g);
    analogWrite(led_b, color.b);
    analogWrite(led_w, color.w);
    //color.print("PWM");
}

void LightRawClass::transition() {
    _mode = 1;
    _startColor = _curColor;
}

void LightRawClass::transition(float scale) {
    int dr, dg, db, dw;
    dr = round(((int)_targetColor.r - _startColor.r) * scale);
    dg = round(((int)_targetColor.g - _startColor.g) * scale);
    db = round(((int)_targetColor.b - _startColor.b) * scale);
    dw = round(((int)_targetColor.w - _startColor.w) * scale);
    _curColor.r = tou8(_startColor.r + dr);
    _curColor.g = tou8(_startColor.g + dg);
    _curColor.b = tou8(_startColor.b + db);
    _curColor.w = tou8(_startColor.w + dw);
    setPWM(_curColor);
}

void LightRawClass::setDirect(const Color &color) {
    if (_mode == 0)
    {
        _curColor = _startColor = color;
        setPWM(color);
    }
    else
    {
        _targetColor = color;
    }
}

void LightRawClass::setRGBW(const Color &color) {
    setDirect(color);
}

void LightRawClass::setRGB(const Color &color) {
    Color modify = color;
    modify.w = 0;
    setRGBW(modify);
}