#pragma once

#include <Joystick.h>
#include <cstdint>
#include <ADS1X15.h>

#ifdef LED
#include <Adafruit_NeoPixel.h>
#endif

#include "Pedal.hpp"
#include "AnalogPedal.hpp"

class Pedals
{
public:
    std::vector<AnalogPedal>& pedals;
    bool updated = false;
    bool inverted = false;

    Joystick_* joystick_ptr;
    ADS1115* ads_ptr;

    Pedals(std::vector<AnalogPedal>& pedals);

    int begin(Joystick_* joystickPtr);
    void update();
    void invert();

#ifdef DEBUG
    void debug_print();
#endif

    #ifdef LED
    uint32_t get_led_colour();
    #endif
};