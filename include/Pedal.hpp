#pragma once

#include <cstdint>
#include <ResponsiveAnalogRead.h>

class Pedal
{
public:
    Pedal(uint16_t adsChannel, int16_t minRawInput, int16_t maxRawInput, float startDeadzone, float endDeadzone);

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
    uint16_t adsChannel = 0;



    virtual void adc_init();
    virtual int64_t adc_read();

    // Maps ADC Channels to Pedals
    enum ePedal
    {
      ACCELERATOR = 26,
      BRAKE = 28,
      CLUTCH = 27
    };

  };


