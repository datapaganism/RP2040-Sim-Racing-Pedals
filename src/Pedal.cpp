#include "Pedal.hpp"
#include "Arduino.h"

void Pedal::readerInit()
{
    Serial.println("Do not call");
}

bool Pedal::read(int64_t &value)
{
    Serial.println("Do not call");
    return false;
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

    this->positiveCoef = (maxRawInput > minRawInput);

    if (useFilter)
    {
        filter = std::make_unique<SimpleKalmanFilter>(filter_e_mea,filter_e_est,filter_q);
    }
}
