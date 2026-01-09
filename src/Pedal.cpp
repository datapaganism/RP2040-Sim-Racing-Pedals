#include "Pedal.hpp"

void Pedal::adc_init()
{
    Serial.println("Do not call");
}

int64_t Pedal::adc_read()
{
    Serial.println("Do not call");
    return 0;
}

void Pedal::class_init()
{
    Serial.println("Do not call");
}

Pedal::Pedal(enum pedalType type, uint16_t adsChannel, int16_t minRawInput, int16_t maxRawInput, float startDeadzone, float endDeadzone)
{
    this->type = type;
    this->adsChannel = adsChannel;
    this->minRawInput = minRawInput;
    this->maxRawInput = maxRawInput;
    
    this->rawRange = maxRawInput - minRawInput;

    this->startDeadzone = startDeadzone * this->rawRange;
    this->endDeadzone = endDeadzone * this->rawRange;

    this->responsiveInput.begin(adsChannel,true);

}

