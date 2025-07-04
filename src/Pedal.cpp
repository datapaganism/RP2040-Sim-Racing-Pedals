#include "Pedal.hpp"

Pedal::Pedal(uint16_t adsChannel, int16_t minRawInput, int16_t maxRawInput, float startDeadzone, float endDeadzone)
{
    this->adsChannel = adsChannel;
    this->minRawInput = minRawInput;
    this->maxRawInput = maxRawInput;
    
    this->rawRange = maxRawInput - minRawInput;

    this->startDeadzone = startDeadzone * this->rawRange;
    this->endDeadzone = endDeadzone * this->rawRange;

    this->smoothedInput.begin(SMOOTHED_AVERAGE, 1);
}