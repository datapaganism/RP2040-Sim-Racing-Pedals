#pragma once

#include "Pedal.hpp"
#include <Arduino.h>

class AnalogPedal : public Pedal
{
public:
  int16_t minRawInputRead = (1 << 10) - 1;

  using Pedal::Pedal;

  void readerInit() override;

  int64_t read() override;

  void oneShotSharedClassInit() override;
};
