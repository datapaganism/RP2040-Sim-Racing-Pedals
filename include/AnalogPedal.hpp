#pragma once

#include "Pedal.hpp"
#include <Arduino.h>


class AnalogPedal : public Pedal
{
public:

  using Pedal::Pedal;

  void adc_init() override;

  int64_t adc_read() override;

  // Maps ADC Channels to Pedals
  enum ePedal
  {
    ACCELERATOR = 26,
    BRAKE = 28,
    CLUTCH = 27
  };

  };


