#include <Pedals.hpp>
#include <Pedal.hpp>

// extern Joystick_ Joystick;

Pedals::Pedals(int number_of_pedals, Pedal *pedal_array)
{
    this->number_of_pedals = number_of_pedals;
    this->pedals = pedal_array;
}

int Pedals::begin(Joystick_* joystickPtr, ADS1115* adsPtr)
{
    if (adsPtr == NULL)
    {
        return 1;
    }

    if (!adsPtr->isConnected())
    {
        return 2;
    }
    this->ads_ptr = adsPtr;

    if (joystickPtr == NULL)
    {
        return 3;
    }
    this->joystick_ptr = joystickPtr;

    return 0;
}

void Pedals::update()
{
    for (int i = 0; i < number_of_pedals; i++)
    {
        pedals[i].currentRawInput = ads_ptr->readADC(pedals[i].adsChannel);


        #if defined(DEBUG)
            if (pedals[i].currentRawInput > pedals[i].maxRawInputRead)
            {
                pedals[i].maxRawInputRead = pedals[i].currentRawInput;
            }

            if (pedals[i].currentRawInput < pedals[i].minRawInputRead)
            {
                pedals[i].minRawInputRead = pedals[i].currentRawInput;
            }
        #endif


        pedals[i].smoothedInput.add(pedals[i].currentRawInput);

        uint16_t smoothed = pedals[i].smoothedInput.get();
        pedals[i].currentOutput = constrain(map(smoothed, pedals[i].minRawInput + pedals[i].startDeadzone, pedals[i].maxRawInput - pedals[i].endDeadzone, pedals[i].minOutput, pedals[i].maxOutput), pedals[i].minOutput, pedals[i].maxOutput );

        if (joystick_ptr != NULL)
        {
            if (pedals[i].adsChannel == ePedal::ACCELERATOR)
            {
                joystick_ptr->Z(pedals[i].currentOutput);
            }

            if (pedals[i].adsChannel == ePedal::BRAKE)
            {  
            joystick_ptr->Zrotate(pedals[i].currentOutput);
            }
            
            if (pedals[i].adsChannel == ePedal::CLUTCH)
            {
                joystick_ptr->slider(pedals[i].currentOutput);
            }
        }
    }
}

uint32_t Pedals::get_led_colour()
{

    uint8_t r = 0;
    uint8_t g = 0;
    uint8_t b = 0;

    for (int i = 0; i < number_of_pedals; i++)
    {

        uint8_t scaled = constrain(map(pedals[i].currentOutput, pedals[i].minOutput, pedals[i].maxOutput, 0, 255 ), 0, 255);

        if (pedals[i].adsChannel == ePedal::ACCELERATOR)
        {
            g = scaled;
        }

        if (pedals[i].adsChannel == ePedal::BRAKE)
        {
            r = scaled;
        }

        if (pedals[i].adsChannel == ePedal::CLUTCH)
        {
            b = scaled;
        }
    }
    return Adafruit_NeoPixel::Color(r,g,b);
}

void Pedals::debug_print()
{
    for (int i = 0; i < number_of_pedals; i++)
    {

        int percentage = constrain(map(pedals[i].currentOutput, pedals[i].minOutput, pedals[i].maxOutput, 0, 100), 0, 100);
        
        const char* as_string;
        if (pedals[i].adsChannel == ePedal::ACCELERATOR)
        {
            as_string = "Accelerator ";
        }

        if (pedals[i].adsChannel == ePedal::BRAKE)
        {  
            as_string = "Brake       ";
        }
        
        if (pedals[i].adsChannel == ePedal::CLUTCH)
        {
            as_string = "Clutch      ";
        }

        Serial.printf("%s- %3i%% Raw: %5i, Min: %5i, Max:%5i, Range:%5i \n", as_string, percentage, pedals[i].currentRawInput, pedals[i].minRawInputRead, pedals[i].maxRawInputRead, (pedals[i].maxRawInputRead - pedals[i].minRawInputRead));
        
    }
    
    Serial.printf("\n");
}