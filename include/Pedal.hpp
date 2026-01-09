#pragma once

#include <cstdint>
#include <ResponsiveAnalogRead.h>

enum pedalType
{
    ACCELERATOR,
    BRAKE,
    CLUTCH
};

class Pedal
{
public:
    Pedal(enum pedalType type, uint16_t pinChannel, int16_t minRawInput, int16_t maxRawInput, float startDeadzone, float endDeadzone);
    
    static inline bool oneShotSharedClassInitDone = false; 
    enum pedalType type;

    int16_t minRawInput = 0;   
    int16_t maxRawInput = 0;   
    int16_t minRawInputRead = (1 << 10) - 1;   
    int16_t maxRawInputRead = 0;   
    int16_t minOutput = 0;   
    int16_t maxOutput = (1 << 10) - 1;   
    int16_t startDeadzone = 0;   
    int16_t endDeadzone = 0;   
    int16_t currentRawInput = 0;
    int16_t lastRawInput = 0;
    int16_t rawRange = 0;   
    ResponsiveAnalogRead responsiveInput;

    int16_t currentOutput = 0;
    uint16_t pinChannel = 0;

    virtual void oneShotSharedClassInit();
    virtual void readerInit();
    virtual int64_t read();
  };

