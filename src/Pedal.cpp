#include "Pedal.hpp"

Pedal::Pedal(uint16_t adsChannel, int16_t minRawInput, int16_t maxRawInput, float startDeadzone, float endDeadzone)
{
    this->adsChannel = adsChannel;
    this->minRawInput = minRawInput;
    this->maxRawInput = maxRawInput;
    
    this->rawRange = maxRawInput - minRawInput;

    this->startDeadzone = startDeadzone * this->rawRange;
    this->endDeadzone = endDeadzone * this->rawRange;

    this->responsiveInput.begin(adsChannel,true);

}

void Pedal::adc_init()
{
}

int64_t Pedal::adc_read()
{
    return 0;
}
