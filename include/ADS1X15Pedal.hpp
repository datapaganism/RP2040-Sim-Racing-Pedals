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



  void adc_init() override;
  int64_t adc_read() override;
  void class_init() override;

  // enum ePedal
  // {
  //   ACCELERATOR = 0,
  //   BRAKE = 1,
  //   CLUTCH = 2
  // };

  };

