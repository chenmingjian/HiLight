#ifndef __DATASTRUCT_HPP__
#define __DATASTRUCT_HPP__

#include <Arduino.h>

#include <cstdint> 

struct Color {
	uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t w;
	Color(uint8_t r = 0, uint8_t g = 0, uint8_t b = 0, uint8_t w = 0) { this->r = r; this->g = g; this->b = b; this->w = w; }
	void print (const char* header = "Color") const {
		Serial.printf("%s: (%02x,%02x,%02x,%02x)\n", header, r, g, b, w);
	}
};

inline Color uint32ToColor(uint32_t value) {
	return Color( (value >> 16) & 0xff, (value >> 8) & 0xff, (value >> 0) & 0xff, (value >> 24) & 0xff );
}

inline uint32_t colorToUint32(const Color& color) {
	return (color.w << 24) | (color.r << 16) | (color.g << 8) | (color.b << 0);
}


#endif
