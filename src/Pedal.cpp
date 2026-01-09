#include "Pedal.hpp"

void Pedal::readerInit()
{
    Serial.println("Do not call");
}

int64_t Pedal::read()
{
    Serial.println("Do not call");
    return 0;
}

void Pedal::oneShotSharedClassInit()
{
    Serial.println("Do not call");
}

Pedal::Pedal(enum pedalType type, uint16_t pinChannel, int16_t minRawInput, int16_t maxRawInput, float startDeadzone, float endDeadzone)
{
    this->type = type;
    this->pinChannel = pinChannel;
    this->minRawInput = minRawInput;
    this->maxRawInput = maxRawInput;
    
    this->rawRange = maxRawInput - minRawInput;

    this->startDeadzone = startDeadzone * this->rawRange;
    this->endDeadzone = endDeadzone * this->rawRange;

    this->responsiveInput.begin(pinChannel,true);

}

