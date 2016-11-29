#ifndef __LIGHTTRANS_HPP__
#define __LIGHTTRANS_HPP__

#include "DataStruct.hpp"

class LightTransClass {
private:
	unsigned int _startTime;
	unsigned int _endTime;
	unsigned int _period;
public:
	LightTransClass() : _period(500) {}
	void targetRGB(const Color &color);
	bool update();
	void period(int value);
};

extern LightTransClass LightTrans;

#endif

