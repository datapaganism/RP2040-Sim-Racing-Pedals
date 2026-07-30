#include "AnalogPedal.hpp"

void AnalogPedal::readerInit()
{
    pinMode(pinChannel, INPUT);
}

bool AnalogPedal::read(int64_t &value)
{
    value = analogRead(pinChannel);
    return true;
}

void AnalogPedal::oneShotSharedClassInit()
{
    analogReadResolution(12);
}