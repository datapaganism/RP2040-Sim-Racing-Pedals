#pragma once

#include "Pedal.hpp"
#include <Arduino.h>

class AnalogPedal : public Pedal
{
public:
  int16_t minRawInputRead = (1 << 12) - 1;

  using Pedal::Pedal;

  void readerInit() override;

  bool read(int64_t &value) override;

  void oneShotSharedClassInit() override;
};
