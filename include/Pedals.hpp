#pragma once

#include <Joystick.h>
#include <cstdint>
#include <ADS1X15.h>
#include <memory>

#ifdef LED
#include <Adafruit_NeoPixel.h>
#endif

#include "Pedal.hpp"
#include "AnalogPedal.hpp"

class Pedals
{
public:
    std::vector<std::unique_ptr<Pedal>> &pedals;
    bool updated = true;
    bool inverted = false;

    Joystick_ *joystick_ptr;
    ADS1115 *ads_ptr;

    Pedals(std::vector<std::unique_ptr<Pedal>> &pedals);

    int begin(Joystick_ *joystickPtr);
    void update();
    void invert();

#ifdef DEBUG
    void debug_print();
#endif

#ifdef LED
    uint32_t get_led_colour();
#endif
};