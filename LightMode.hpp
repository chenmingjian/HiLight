#ifndef __LIGHTMODE_HPP__
#define __LIGHTMODE_HPP__

#include "LightRaw.hpp"
#include <vector>
#include <array>

class LightModeHandler {
public:
    static constexpr int maxParms = 64;
    virtual const char* name()=0;
    virtual bool begin() = 0;
    virtual void loop()=0;
    bool begin(int count, uint32_t parms[]) {
    	if (count > maxParms)
    		return false;
    	this->parms.resize(count);
    	uint32_t *data = this->parms.data();
    	memcpy(data, parms, count*sizeof(uint32_t));
    	return begin();
    };

    bool begin(std::vector<uint32_t> &&parms) {
    	this->parms = std::move(parms);
    	return begin();
    }
    bool begin(const std::vector<uint32_t> &parms) {
    	this->parms = parms;
    	return begin();
    }
    std::vector<uint32_t>& getParms() { return parms; }
//  virtual void exit() = 0;
protected:
	std::vector<uint32_t> parms;
};

extern std::array<LightModeHandler*, 2> LightModeHandlers;

#endif

