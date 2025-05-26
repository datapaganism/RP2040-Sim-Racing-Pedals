#include <Arduino.h>
#include <Wire.h>
#include <Joystick.h>
#include <ADS1X15.h>

#ifdef LED
#include <Adafruit_NeoPixel.h>
#endif

#include "Pedals.hpp"
#include "Pedal.hpp"

// Define Pedal values in this array, comment out a Pedal if not needed.
Pedal pedal_array[] = {
    Pedal(ePedal::ACCELERATOR, 23900, 25470, 0.05, 0.03),
    Pedal(ePedal::BRAKE, 3100, 9264, 0.03, 0.01),
    Pedal(ePedal::CLUTCH, 9125, 11210, 0.05, 0.05)};
constexpr int number_of_pedals = sizeof(pedal_array) / sizeof(pedal_array[0]);




#ifdef LED
Adafruit_NeoPixel pixels(1, 23, NEO_GRB + NEO_KHZ800);
#endif

ADS1115 ADS(0x48);
Pedals pedals(number_of_pedals, pedal_array);

#ifdef DEBUG
bool sendDebug = true;
#endif

// A User button has been defined, allowing you to invert all of the pedals in one go,
// define the button and the LED to show its progress.
// Useful for old games that don't let you invert
#define INVERT_BUTTON 24
#define INVERT_BUTTON_LED 25

static uint8_t previous_toggle_invert = 0;
static uint8_t toggle_invert = 0;

const float debug_refresh_rate_hz = DEBUG_HZ;
const float report_refresh_rate_hz = REPORT_HZ;

const int debug_refresh_rate_ms = ((1 / debug_refresh_rate_hz) * 1000);
const int report_refresh_rate_ms = ((1 / report_refresh_rate_hz) * 1000);

unsigned long current_millis = 0, last_debug = 0, last_report = 0;

static void clear_serial_monitor()
{
    Serial.write(27);    // ESC
    Serial.print("[2J"); // Clear entire screen
    Serial.write(27);    // ESC
    Serial.print("[H");  // Move cursor to home position
}

void setup()
{
#ifdef DEBUG
    Serial.begin(MONITOR_SPEED);
#endif

#ifdef LED
    pixels.begin();
    pixels.setBrightness(255);
#endif

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
#ifdef LED
            pixels.setBrightness(120);
            pixels.setPixelColor(0, pixels.Color(255, 0, 0));
            pixels.show();
            delay(300);
            pixels.setPixelColor(0, pixels.Color(128, 0, 0));
            pixels.show();
            delay(300);
#endif
        }
    }

    int ret = pedals.begin(&Joystick, &ADS);
    if (ret != 0)
    {
        while (1)
        {
            Serial.printf("problem with init %i\n", ret);
#ifdef LED
            pixels.setBrightness(120);
            pixels.setPixelColor(0, pixels.Color(0, 255, 0));
            pixels.show();
            delay(300);
            pixels.setPixelColor(0, pixels.Color(0, 128, 0));
            pixels.show();
            delay(300);
#endif
        }
    }
}

void loop()
{
    current_millis = millis();

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

#ifdef DEBUG
    if (current_millis - last_debug > debug_refresh_rate_ms)
    {
        sendDebug = true;
        clear_serial_monitor();
        pedals.debug_print();
        last_debug = current_millis;
    }
#endif

    if (current_millis - last_report > report_refresh_rate_ms)
    {
        pedals.update();
        last_report = current_millis;
        if (pedals.updated)
        {
            Joystick.send_now();
            pedals.updated = false;
        }

#ifdef LED
        pixels.setPixelColor(0, pedals.get_led_colour());
        pixels.show();
#endif

        if (toggle_invert != previous_toggle_invert)
        {
            digitalWrite(INVERT_BUTTON_LED, toggle_invert);
            previous_toggle_invert = toggle_invert;
        }
    }

#ifdef DEBUG
    sendDebug = false;
#endif
}