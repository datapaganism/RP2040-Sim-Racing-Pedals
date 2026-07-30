#include "ADS1X15Pedal.hpp"

#include <ADS1X15.h>

ADS1115 ADS(0x48);

extern TwoWire Wire;
extern void flash_error();

void ADS1X15Pedal::oneShotSharedClassInit()
{
    Wire.setSDA(20);
    Wire.setSCL(21);
    Wire.begin();
    Wire.setClock(400000);

    ADS.begin();
    ADS.setGain(1);
    ADS.setDataRate(ADS1115_860_SPS);

    //     if (!ADS.isConnected())
    //     {
    //         while (1)
    //         {
    //             Serial.printf("ADC NOT CONNECTED\n");
    // #ifdef LED
    //             flash_error();
    // #endif
    //         }
    //     }
}

void ADS1X15Pedal::readerInit()
{

}

bool ADS1X15Pedal::read(int64_t &value)
{

    int64_t read = ADS.readADC(pinChannel);
    if (ADS.getError() != ADS1X15_OK)
    {
        return false;
    }
    value = read;
    return true;
}