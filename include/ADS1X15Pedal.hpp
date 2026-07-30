#pragma once

#include "Pedal.hpp"

class ADS1X15Pedal : public Pedal
{
public:
  using Pedal::Pedal;

  int16_t minRawInputRead = (1 << 15) - 1;
  int16_t maxRawInputRead = 0;
  int16_t minOutput = -(1 << 15);
  int16_t maxOutput = (1 << 15) - 1;

  void readerInit() override;
  bool read(int64_t &value) override;
  void oneShotSharedClassInit() override;
};
