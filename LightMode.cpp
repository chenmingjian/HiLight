#include <Arduino.h>
#include "LightMode.hpp"
#include "LightTrans.hpp"

using namespace std;

class StaticHandler : LightModeHandler {
public:
    StaticHandler() {}
    const char* name() { return "static"; }
    virtual void loop(){
        if (stable)
            return;
        if (LightTrans.update())
            stable = true;
    }
    virtual bool begin()
    {
        if (parms.size() == 2 && parms[0] == 0)
        {
            LightTrans.period(150);
            LightTrans.targetRGB(uint32ToColor(parms[1]));
            stable = false;
            return true;
        }
        return false;
    }
private:
    bool stable;
};

class FlowHandler : LightModeHandler {
private:
    int index;
    inline Color getNextColor() {
        index = 1 + (index % (parms.size() - 1));
        Serial.printf("size = %d, index = %d\n", parms.size(), index);
        return getCurrentColor();
    }
    inline Color getCurrentColor() {
        return uint32ToColor(parms[index]);
    }
public:
    const char* name() { return "flow"; }
    virtual bool begin()
    {
        index = 0;
        LightTrans.period(parms[0]);
        LightTrans.targetRGB(getNextColor());
        return true;
    }
    virtual void loop()
    {
        if (LightTrans.update())
        {
            LightTrans.targetRGB(getNextColor());
        }
    }
};

//LightModeHandler *modeHandlerPtr = (LightModeHandler*)&_test;

static StaticHandler _staticHandler;
static FlowHandler   _flowHandler;

array<LightModeHandler*, 2> LightModeHandlers{{ (LightModeHandler*)&_staticHandler, (LightModeHandler*)&_flowHandler }};