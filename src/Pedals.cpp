#include <Pedals.hpp>
#include <Pedal.hpp>

// extern Joystick_ Joystick;

Pedals::Pedals(std::vector<std::unique_ptr<Pedal>> &pedals) : pedals(pedals) {}

int Pedals::begin(Joystick_ *joystickPtr)
{
    if (joystickPtr == NULL)
    {
        return 3;
    }
    this->joystick_ptr = joystickPtr;

    return 0;
}

void Pedals::update()
{
    for (const auto &pedal : pedals)
    {
        pedal->currentRawInput = pedal->read();

        if ( (!pedal->useFilter) && (pedal->currentRawInput == pedal->lastRawInput))
        {
            continue;
        }
        pedal->lastRawInput = pedal->currentRawInput;
        this->updated = true;

#ifdef DEBUG
        if (pedal->currentRawInput > pedal->maxRawInputRead)
        {
            pedal->maxRawInputRead = pedal->currentRawInput;
        }

        if (pedal->currentRawInput < pedal->minRawInputRead)
        {
            pedal->minRawInputRead = pedal->currentRawInput;
        }
#endif

        int16_t smoothed = pedal->currentRawInput;
        if (pedal->useFilter)
        {
            smoothed = (int16_t)pedal->filter->updateEstimate(pedal->currentRawInput);
        }

        int16_t mapMin = (this->inverted) ? pedal->maxOutput : pedal->minOutput;
        int16_t mapMax = (this->inverted) ? pedal->minOutput : pedal->maxOutput;
        pedal->currentOutput = constrain(map(smoothed, pedal->minRawInput + pedal->startDeadzone, pedal->maxRawInput - pedal->endDeadzone, mapMin, mapMax), pedal->minOutput, pedal->maxOutput);
        if (joystick_ptr != NULL)
        {
            if (pedal->type == pedalType::ACCELERATOR)
            {
                joystick_ptr->Z(pedal->currentOutput);
            }

            if (pedal->type == pedalType::BRAKE)
            {
                joystick_ptr->Zrotate(pedal->currentOutput);
            }

            if (pedal->type == pedalType::CLUTCH)
            {
                joystick_ptr->slider(pedal->currentOutput);
            }
        }
    }
}
#ifdef LED
uint32_t Pedals::get_led_colour()
{
    uint8_t r = 0;
    uint8_t g = 0;
    uint8_t b = 0;

    for (const auto &pedal : pedals)
    {

        uint8_t mapMin = (this->inverted) ? 255 : 0;
        uint8_t mapMax = (this->inverted) ? 0 : 255;

        uint8_t scaled = constrain(map(pedal->currentOutput, pedal->minOutput, pedal->maxOutput, mapMin, mapMax), 0, 255);

        if (pedal->type == pedalType::ACCELERATOR)
        {
            g = scaled;
        }

        if (pedal->type == pedalType::BRAKE)
        {
            r = scaled;
        }

        if (pedal->type == pedalType::CLUTCH)
        {
            b = scaled;
        }
    }
    return Adafruit_NeoPixel::Color(r, g, b);
}
#endif

#ifdef DEBUG
void Pedals::debug_print()
{
    for (const auto &pedal : pedals)
    {

        int percentage = constrain(map(pedal->currentOutput, pedal->minOutput, pedal->maxOutput, 0, 100), 0, 100);

        const char *as_string;
        if (pedal->type == pedalType::ACCELERATOR)
        {
            as_string = "Accelerator ";
        }

        if (pedal->type == pedalType::BRAKE)
        {
            as_string = "Brake       ";
        }

        if (pedal->type == pedalType::CLUTCH)
        {
            as_string = "Clutch      ";
        }

        Serial.printf("%s- %3i%% (%5i), Raw: %5i, Min: %5i, Max: %5i, Min-Dead: %5i, Max-Dead: %5i,  Range: %5i \n", as_string, percentage, pedal->currentOutput, pedal->currentRawInput, pedal->minRawInputRead, pedal->maxRawInputRead, pedal->minRawInput + pedal->startDeadzone, pedal->maxRawInput - pedal->endDeadzone, (pedal->maxRawInputRead - pedal->minRawInputRead));
    }

    Serial.printf("\n");
}
#endif

void Pedals::invert()
{
    this->inverted = !this->inverted;
}