#pragma once

#include <Joystick.h>
#include <Smoothed.h>
#include <cstdint>
#include <ADS1X15.h>


class Pedal
{
public:
    uint16_t minRawInput = 0;   
    uint16_t maxRawInput = 0;   
    uint16_t minRawInputRead = (1 << 16) - 1;   
    uint16_t maxRawInputRead = 0;   
    uint16_t minOutput = 0;   
    uint16_t maxOutput = (1 << 10) - 1;   
    uint16_t startDeadzone = 0;   
    uint16_t endDeadzone = 0;   
    uint16_t currentRawInput = 0;   
    Smoothed<int16_t> smoothedInput;

    uint16_t currentOutput = 0;
    
    uint16_t adsChannel = 0;


    Pedal(uint16_t adsChannel, uint16_t minRawInput, uint16_t maxRawInput, uint16_t startDeadzone, uint16_t endDeadzone);

};

enum Pedal_to_ADS
{
  ACC = 0,
  BRAKE = 1,
  CLUTCH = 2
};