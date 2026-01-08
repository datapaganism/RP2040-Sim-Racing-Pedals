#pragma once

#include "Pedal.hpp"

class ADS1X15Pedal : public Pedal
{
public:

  using Pedal::Pedal;


  void adc_init();
  int64_t adc_read();

  enum ePedal
  {
    ACCELERATOR = 0,
    BRAKE = 1,
    CLUTCH = 2
  };

  };

