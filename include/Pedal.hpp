#pragma once

#include <Joystick.h>
#include <Smoothed.h>
#include <cstdint>
#include <ADS1X15.h>


class Pedal
{
public:
    int16_t minRawInput = 0;   
    int16_t maxRawInput = 0;   
    int16_t minRawInputRead = (1 << 15) - 1;   
    int16_t maxRawInputRead = 0;   
    int16_t minOutput = -(1 << 15);   
    int16_t maxOutput = (1 << 15) - 1;   
    int16_t startDeadzone = 0;   
    int16_t endDeadzone = 0;   
    int16_t currentRawInput = 0;
    int16_t lastRawInput = 0;
    int16_t rawRange = 0;   
    Smoothed<int16_t> smoothedInput;

    int16_t currentOutput = 0;
    
    uint16_t adsChannel = 0;


    Pedal(uint16_t adsChannel, int16_t minRawInput, int16_t maxRawInput, float startDeadzone, float endDeadzone);

};

// Maps ADC Channels to Pedals
enum ePedal
{
  ACCELERATOR = 0,
  BRAKE = 1,
  CLUTCH = 2
};