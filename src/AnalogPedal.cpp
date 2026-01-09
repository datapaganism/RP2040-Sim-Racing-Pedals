#include "AnalogPedal.hpp"



void AnalogPedal::readerInit()
{
    pinMode(pinChannel,INPUT);
}

int64_t AnalogPedal::read()
{
    
    return analogRead(pinChannel);
}

void AnalogPedal::oneShotSharedClassInit()
{
    analogReadResolution(10);
}