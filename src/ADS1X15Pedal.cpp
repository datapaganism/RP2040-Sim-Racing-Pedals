#include "ADS1X15Pedal.hpp"

#include <ADS1X15.h>


extern ADS1115 ADS;

void ADS1X15Pedal::adc_init() 
{

}

int64_t ADS1X15Pedal::adc_read()
{
    int64_t read = ADS.readADC(adsChannel);
    if (read == ADS1X15_ERROR_TIMEOUT)
    {
    #ifdef DEBUG
        Serial.printf("timeout\n");
    #endif
        return 0;
    }
    return read;
}
