#include <Arduino.h>
#include "LightTrans.hpp"
#include "LightRaw.hpp"

void LightTransClass::targetRGB(const Color &color) {
	_startTime = millis();
	_endTime = _startTime + _period;
	LightRaw.transition();
	LightRaw.setRGB(color);
}

void LightTransClass::period(int value) {
	_period = value;
} 

bool LightTransClass::update() {
	unsigned int cur = millis();
	if (cur >= _endTime) {
		LightRaw.transition(1);
		LightRaw.stable();
		return true;
	}
	float scale = ((float)(cur - _startTime)) / (_endTime - _startTime);
	LightRaw.transition(scale);
	return false;
}  

LightTransClass LightTrans;