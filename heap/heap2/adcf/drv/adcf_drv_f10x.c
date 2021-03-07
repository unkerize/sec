
static uint8_t   adc_os_indx;
static uint8_t*  adc_chls;
static uint16_t* adc_buf;
static uint8_t   adc_chn;
static uint8_t   adc_flt_deep;
static uint8_t   adc_flt_deci;
static uint16_t  adc_flt_size;

static uint16_t  adc_flt_cnt = 0;
static uint8_t   adc_ch_cnt = 0;

static fptr      adc_handler = 0;
static uint8_t   adc_tag = 0;

void ADC1_IRQHandler(void)
{
  uint32_t* sum;

  sum = (uint32_t*) &adc_buf[adc_chn + 2*adc_ch_cnt];

  *sum += ADC1->DR;

  if (++adc_flt_cnt == adc_flt_size) stSetEvent(adc_os_indx);
    else ADC1->CR2 |= ADC_CR2_ADON;
}


static void adcf_main(void)
{
  uint32_t* sum;

  sum = (uint32_t*) &adc_buf[adc_chn + 2*adc_ch_cnt];

  adc_buf[adc_ch_cnt] = *sum >> (adc_flt_deep - adc_flt_deci);

  *sum = 0;
  adc_flt_cnt = 0;

  if (++adc_ch_cnt == adc_chn) { adc_ch_cnt = 0; if (adc_handler) adc_handler();}

  ADC1->SQR3 = adc_chls[adc_ch_cnt] & 0x1f;

  ADC1->CR2 |= ADC_CR2_ADON;
}


uint16_t* adcf_add(uint8_t* chls, uint8_t chn, uint8_t flt_deep, uint8_t flt_deci, fptr ev_handler)
{
  uint16_t buf_size;
  uint8_t i;

  if (adc_tag) sysErrHandler(ser_adc_cross);

  adc_tag = 1; // tag the selected ADC

  adc_handler = ev_handler;

  RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;

  ADC1->CR2 = ADC_CR2_TSVREFE ;

  // check params
  if ( (chls == 0) || (chn == 0) || (chn > 16) || (flt_deep > 16) || (flt_deep < 1) || (flt_deci >= flt_deep) ) sysErrHandler(ser_par_err);

  ADC1->CR2 |= ADC_CR2_CAL | ADC_CR2_ADON; // start calibration

  adc_chn  = chn;
        adc_chls = chls;
  adc_flt_deep = flt_deep;
  adc_flt_deci = flt_deci;
  adc_flt_size = 1<<flt_deep;

  buf_size = adc_chn*2 + adc_chn*4 + adc_chn*4;

  adc_buf = malloc(buf_size);

  if ( adc_buf == 0 ) sysErrHandler(ser_heap_err);

  while ( (ADC1->CR2 & ADC_CR2_CAL) != 0) {}

  for (i = 0; i < ADCF_MAX_CHN; i++)
  {
    if (i < 10 ) ADC1->SMPR2 |= ADCF_SAMPLE_CONST << (i*3);

    if ( (i >= 10) && (i < ADCF_MAX_CHN ) ) ADC1->SMPR1 |= ADCF_SAMPLE_CONST << ( (i-10)*3);
  }

  ADC1->SQR1 = 0;
  ADC1->CR1 = ADC_CR1_EOCIE;
        NVIC_EnableIRQ(ADC1_IRQn);

  ADC1->SQR3 = adc_chls[adc_ch_cnt] & 0x1f;
  ADC1->CR2 |= ADC_CR2_ADON;

  adc_os_indx = stAddTask(&adcf_main, stf_event);

  return adc_buf;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
