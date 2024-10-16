#include <Arduino.h>
#include <Wire.h>
#include <Joystick.h>
#include <Adafruit_NeoPixel.h>
#include <ADS1X15.h>

#include "Pedals.hpp"
#include "Pedal.hpp"

// Define Pedal values in this array, comment out a Pedal if not needed.
Pedal pedal_array[] = { 
  Pedal(ePedal::ACCELERATOR,  6287, 8661 , 0.05, 0.03 ),
  Pedal(ePedal::BRAKE, 3100, 6731, 0.07, 0.01 ),
  Pedal(ePedal::CLUTCH, 4546, 6471, 0.05, 0.05 )
};

int number_of_pedals = sizeof(pedal_array) / sizeof(pedal_array[0]);

Adafruit_NeoPixel pixels(1, 23, NEO_GRB + NEO_KHZ800);
ADS1115 ADS(0x48);

Pedals pedals(number_of_pedals, pedal_array);

bool sendDebug = true;
#define DebugRefreshRate 200
unsigned long currentMillis = 0, lastDebug = 0;

static void clear_serial_monitor()
{
  Serial.write(27);     // ESC
  Serial.print("[2J");  // Clear entire screen
  Serial.write(27);     // ESC
  Serial.print("[H");   // Move cursor to home position
}

void setup()
{

  #if defined(DEBUG)
    Serial.begin(MONITOR_SPEED);
  #endif

  pixels.begin();
  pixels.setBrightness(255);

  Wire.setSDA(20);
  Wire.setSCL(21);
  Wire.begin();
  Wire.setClock(400000);

  Joystick.begin();
  Joystick.use8bit(false);
  Joystick.useManualSend(true);



  ADS.begin();
  ADS.setGain(1);
  ADS.setMode(0);
  ADS.setDataRate(7);

  if (!ADS.isConnected()) {
    while(1)
    {
      Serial.printf("ADC NOT CONNECTED\n");
      pixels.setBrightness(120);
      pixels.setPixelColor(0, pixels.Color(255,0,0));
      pixels.show();
      delay(300);
      pixels.setPixelColor(0, pixels.Color(128,0,0));
      pixels.show();
      delay(300);
    }
    
  }  

  int ret = pedals.begin(&Joystick, &ADS); 
  if (ret != 0)
  {
    while(1)
    {
      Serial.printf("problem with init %i\n", ret);
      pixels.setBrightness(120);
      pixels.setPixelColor(0, pixels.Color(0,255,0));
      pixels.show();
      delay(300);
      pixels.setPixelColor(0, pixels.Color(0,128,0));
      pixels.show();
      delay(300);
    }
  }


}



void loop()
{

pedals.update();

#if defined(DEBUG)
  currentMillis = millis();
  if (currentMillis - lastDebug > DebugRefreshRate )
  {
    sendDebug = true;
    clear_serial_monitor();
    pedals.debug_print();
    lastDebug = currentMillis;
  }
#endif


#if defined(LED)
    // pixels.clear();
    pixels.setPixelColor(0, pedals.get_led_colour());

    pixels.show();
#endif

  if (pedals.updated)
  {
    Joystick.send_now();
    pedals.updated = false;
  }

#if defined(DEBUG)
  sendDebug = false;
#endif

}