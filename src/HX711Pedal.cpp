#include "HX711Pedal.hpp"

HX711Pedal::HX711Pedal(enum pedalType type, uint16_t data, uint16_t clock, int16_t minRawInput, int16_t maxRawInput, float startDeadzone, float endDeadzone) : Pedal(type, 0, minRawInput, maxRawInput, startDeadzone, endDeadzone)
{
    data = data;
    clock = clock;
}

void HX711Pedal::oneShotSharedClassInit()
{
}

void HX711Pedal::readerInit()
{
    scale.begin(data, clock);
    scale.set_scale(280);
    scale.tare();
    this->minRawInput = 0;
    this->minRawInputRead = 0;
}

bool HX711Pedal::read(int64_t &value)
{
    if (scale.is_ready())
    {
        value = scale.get_units();
        return true;
    }
    return false;
}