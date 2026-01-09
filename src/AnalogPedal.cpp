#include "AnalogPedal.hpp"



void AnalogPedal::adc_init()
{
    pinMode(adsChannel,INPUT);
}

int64_t AnalogPedal::adc_read()
{
    
    return analogRead(adsChannel);
}

void AnalogPedal::class_init()
{
    analogReadResolution(10);
}