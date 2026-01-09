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
    scale.tare();
    scale.set_scale(-10000);
}

int64_t HX711Pedal::read()
{
    return -scale.get_units();
}
