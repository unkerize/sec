
#include "adc.h"   


#define ADC_CHAN_NUM 2

const uint8_t adc_channels[ADC_CHAN_NUM] = {0,1};
uint16_t* adc_buff;


        adc_buff = adc_add(ADC1, (uint8_t*) adc_channels, ADC_CHAN_NUM, 0, 0, 0 , 0x07); //&ledPulse1


 adc_buff[0]
