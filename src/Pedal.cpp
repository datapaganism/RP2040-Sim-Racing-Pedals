#include "Pedal.hpp"

Pedal::Pedal(uint16_t adsChannel, uint16_t minRawInput, uint16_t maxRawInput, uint16_t startDeadzone, uint16_t endDeadzone)
{
    this->adsChannel = adsChannel;
    this->minRawInput = minRawInput;
    this->maxRawInput = maxRawInput;
    this->startDeadzone = startDeadzone;
    this->endDeadzone = endDeadzone;

    this->smoothedInput.begin(SMOOTHED_AVERAGE, 1);
}