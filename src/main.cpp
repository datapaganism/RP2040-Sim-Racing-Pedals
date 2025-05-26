#include <Arduino.h>
#include <Wire.h>
#include <Joystick.h>
#include <Adafruit_NeoPixel.h>
#include <ADS1X15.h>

#include "Pedals.hpp"
#include "Pedal.hpp"

// A User button has been defined, allowing you to invert all of the pedals in one go,
// define the button and the LED to show its progress.
// Useful for old games that don't let you invert
#define INVERT_BUTTON 24
#define INVERT_BUTTON_LED 25
static uint8_t toggle_invert=0;

// Define Pedal values in this array, comment out a Pedal if not needed.
Pedal pedal_array[] = {
    Pedal(ePedal::ACCELERATOR, 23900, 25470, 0.05, 0.03),
    Pedal(ePedal::BRAKE, 3100, 9264, 0.03, 0.01),
    Pedal(ePedal::CLUTCH, 9125, 11210, 0.05, 0.05)
};
constexpr int number_of_pedals = sizeof(pedal_array) / sizeof(pedal_array[0]);

Adafruit_NeoPixel pixels(1, 23, NEO_GRB + NEO_KHZ800);
ADS1115 ADS(0x48);

Pedals pedals(number_of_pedals, pedal_array);

bool sendDebug = true;
#define DebugRefreshRate 200
unsigned long currentMillis = 0, lastDebug = 0;

static void clear_serial_monitor()
{
  Serial.write(27);    // ESC
  Serial.print("[2J"); // Clear entire screen
  Serial.write(27);    // ESC
  Serial.print("[H");  // Move cursor to home position
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
  Joystick.use16bit();
  Joystick.useManualSend(true);

  ADS.begin();
  ADS.setGain(1);
  ADS.setMode(0);
  ADS.setDataRate(7);

  pinMode(INVERT_BUTTON, INPUT_PULLUP);
  pinMode(INVERT_BUTTON_LED, OUTPUT);

  if (!ADS.isConnected())
  {
    while (1)
    {
      Serial.printf("ADC NOT CONNECTED\n");
      pixels.setBrightness(120);
      pixels.setPixelColor(0, pixels.Color(255, 0, 0));
      pixels.show();
      delay(300);
      pixels.setPixelColor(0, pixels.Color(128, 0, 0));
      pixels.show();
      delay(300);
    }
  }

  int ret = pedals.begin(&Joystick, &ADS);
  if (ret != 0)
  {
    while (1)
    {
      Serial.printf("problem with init %i\n", ret);
      pixels.setBrightness(120);
      pixels.setPixelColor(0, pixels.Color(0, 255, 0));
      pixels.show();
      delay(300);
      pixels.setPixelColor(0, pixels.Color(0, 128, 0));
      pixels.show();
      delay(300);
    }
  }
}

void loop()
{

  if (!digitalRead(INVERT_BUTTON))
  {
    delay(100);
    if (!digitalRead(INVERT_BUTTON))
    { 
      toggle_invert = !toggle_invert;
      pedals.invert();
    }
    // while (!digitalRead(INVERT_BUTTON))
  }

  pedals.update();

#if defined(DEBUG)
  currentMillis = millis();
  if (currentMillis - lastDebug > DebugRefreshRate)
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

  digitalWrite(INVERT_BUTTON_LED, toggle_invert);

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