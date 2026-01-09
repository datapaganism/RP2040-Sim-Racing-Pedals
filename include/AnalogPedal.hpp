#pragma once

#include "Pedal.hpp"
#include <Arduino.h>


class AnalogPedal : public Pedal
{
public:

  int16_t minRawInputRead = (1 << 10) - 1;   


  using Pedal::Pedal;

  void adc_init() override;

  int64_t adc_read() override;

  void class_init() override;

  // enum ePedal
  // {
  //   ACCELERATOR = 26,
  //   BRAKE = 28,
  //   CLUTCH = 27
  // };

  };


