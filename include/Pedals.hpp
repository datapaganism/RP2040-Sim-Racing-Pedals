#pragma once

#include <Joystick.h>
#include <cstdint>
#include <ADS1X15.h>

#ifdef LED
#include <Adafruit_NeoPixel.h>
#endif

#include "Pedal.hpp"

class Pedals
{
public:
    int number_of_pedals;
    Pedal* pedals;
    bool updated = false;
    bool inverted = false;

    Joystick_* joystick_ptr;
    ADS1115* ads_ptr;

    Pedals(int number_of_pedals, Pedal* pedal_array);

    int begin(Joystick_* joystickPtr, ADS1115* adsPtr);
    void update();
    void invert();

#ifdef DEBUG
    void debug_print();
#endif

    #ifdef LED
    uint32_t get_led_colour();
    #endif
};