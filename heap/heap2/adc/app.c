#define ADC_CHAN_NUM 2

const uint8_t adc_channels[ADC_CHAN_NUM] = {0,1};
uint16_t* adc_buf;

        vis_out(adc_buf[1]>>0, 4);


	adc_buf = adc_add(ADC1, (uint8_t*) adc_channels, ADC_CHAN_NUM, 3, 0);
