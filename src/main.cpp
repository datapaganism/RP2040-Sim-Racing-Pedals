#include <Arduino.h>
#include <Wire.h>
#include <Joystick.h>
#include <ADS1X15.h>
#include <vector>

#ifdef LED
#include <Adafruit_NeoPixel.h>
#endif

#include "Pedals.hpp"
#include "Pedal.hpp"
#include "AnalogPedal.hpp"


// Define Pedal values in this array, comment out a Pedal if not needed.
std::vector<AnalogPedal> pedal_array = {
    AnalogPedal(AnalogPedal::ePedal::ACCELERATOR, 1023, 620, 0.00, 0.04),
    AnalogPedal(AnalogPedal::ePedal::BRAKE, 1023, 530, 0.00, 0.04),
    AnalogPedal(AnalogPedal::ePedal::CLUTCH, 1023, 530, 0.05, 0.05)
};

#ifdef LED
    Adafruit_NeoPixel pixels(1, 23, NEO_GRB + NEO_KHZ800);
#endif

ADS1115 ADS(0x48);
Pedals pedals(pedal_array);

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

#ifdef LED
static void flash_error()
{
    pixels.setBrightness(120);
    pixels.setPixelColor(0, pixels.Color(255, 0, 0));
    pixels.show();
    delay(300);
    pixels.setPixelColor(0, pixels.Color(128, 0, 0));
    pixels.show();
    delay(300);
}
#endif

static void clear_serial_monitor()
{
    Serial.write(27);    // ESC
    Serial.print("[2J"); // Clear entire screen
    Serial.write(27);    // ESC
    Serial.print("[H");  // Move cursor to home position
}

void handle_invert_interrupt()
{
    toggle_invert = !toggle_invert;
    pedals.invert();
}

void ads_init()
{
    Wire.setSDA(20);
    Wire.setSCL(21);
    Wire.begin();
    Wire.setClock(400000);

    ADS.begin();
    ADS.setGain(1);
    ADS.setMode(1);
    ADS.setDataRate(7);

    if (!ADS.isConnected())
    {
        while (1)
        {
            Serial.printf("ADC NOT CONNECTED\n");
#ifdef LED
            flash_error();
#endif
        }
    }

}

void analog_init()
{
    analogReadResolution(10);
}

void hx711_init()
{

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


    analog_init();

    Joystick.begin();
    Joystick.use10bit();
    Joystick.useManualSend(true);

    attachInterrupt(digitalPinToInterrupt(INVERT_BUTTON), handle_invert_interrupt, FALLING);

    pinMode(INVERT_BUTTON, INPUT_PULLUP);
    pinMode(INVERT_BUTTON_LED, OUTPUT);

    int ret = pedals.begin(&Joystick);
    if (ret != 0)
    {
        while (1)
        {
            Serial.printf("problem with init %i\n", ret);
#ifdef LED
            flash_error();
#endif
        }
    }

    for (auto pedal : pedal_array)
    {
        pedal.adc_init();
    }
}


void loop()
{
    current_millis = millis();

#ifdef DEBUG
    if (current_millis - last_debug > debug_refresh_rate_ms)
    {
        sendDebug = true;
        clear_serial_monitor();
        pedals.debug_print();
        last_debug = current_millis;
    }
#endif

    pedals.update();

    if (current_millis - last_report > report_refresh_rate_ms)
    {
        last_report = current_millis;
        // if (pedals.updated)
        // {
            Joystick.send_now();
            pedals.updated = false;
        // }

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