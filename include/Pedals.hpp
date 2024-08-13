#pragma once

#include <Joystick.h>
#include <cstdint>
#include <ADS1X15.h>
#include <Adafruit_NeoPixel.h>

#include "Pedal.hpp"

class Pedals
{
public:
    int number_of_pedals;
    Pedal* pedals;

    Joystick_* joystick_ptr;
    ADS1115* ads_ptr;

    Pedals(int number_of_pedals, Pedal* pedal_array);

    int begin(Joystick_* joystickPtr, ADS1115* adsPtr);
    void update();
    void debug_print();

    uint32_t get_led_colour();
};