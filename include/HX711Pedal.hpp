#pragma once

#include "Pedal.hpp"
#include <HX711.h>

class HX711Pedal : public Pedal
{
public:
  HX711Pedal(enum pedalType type, uint16_t data, uint16_t clock, int16_t minRawInput, int16_t maxRawInput, float startDeadzone, float endDeadzone);

  HX711 scale;

  uint8_t data = 16;
  uint8_t clock = 17;

  int16_t minRawInputRead = (1 << 15) - 1;
  int16_t maxRawInputRead = 0;
  int16_t minOutput = -(1 << 15);
  int16_t maxOutput = (1 << 15) - 1;

  bool useFilter = false;

  void readerInit() override;
  bool read(int64_t &value) override;
  void oneShotSharedClassInit() override;
};
