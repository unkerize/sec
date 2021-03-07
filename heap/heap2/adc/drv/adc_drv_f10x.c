
static fptr      adc_dm_handler = 0;
static uint8_t   adc_dm_tag = 0;


typedef struct {
  uint8_t  osComIndex;

  ADC_TypeDef* port;
  DMA_Channel_TypeDef* dma;
        uint16_t* rbuf;
        uint8_t chn;
        uint8_t flt_deep;
        uint8_t flt_deci;
} adcType;

uint8_t adc_counter = 0;

adcType adc[ADC_NUMBER];

uint8_t adc_handler[ADC_NUMBER];

void DMA1_Channel1_IRQHandler(void)
{
  DMA1->IFCR = DMA_IFCR_CTCIF1;

  if (adc_dm_handler) adc_dm_handler();
}


uint16_t* adcf_add(uint8_t* chls, uint8_t chn, fptr ev_handler)
{
  uint16_t  buf_size, i, flt;
  uint16_t* buf;

  if (adc_dm_tag) sysErrHandler(ser_adc_cross);

  adc_dm_tag = 1; // tag the selected ADC

  adc_dm_handler = ev_handler;

  RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
  RCC->AHBENR  |= RCC_AHBENR_DMA1EN;


  ADC1->CR2 = ADC_CR2_TSVREFE ;

  // check params
  if ( (chls == 0) || (chn == 0) || (chn > 16) ) sysErrHandler(ser_par_err);

  ADC1->CR2 |= ADC_CR2_CAL | ADC_CR2_ADON; // start calibration

  buf_size = chn * 2;

  buf = malloc(buf_size);

  if (buf == 0 ) sysErrHandler(ser_heap_err);

  if (DMA1_Channel1->CCR != 0)  sysErrHandler(ser_dma_cross);

  DMA1_Channel1->CPAR  = (uint32_t) &ADC1->DR;
  DMA1_Channel1->CMAR  = (uint32_t) buf;
  DMA1_Channel1->CNDTR = buf_size/2;

  if (adc_dm_handler)
  {
    DMA1_Channel1->CCR = DMA_CCR1_MINC |  DMA_CCR1_MSIZE_0 | DMA_CCR1_PSIZE_1 | DMA_CCR1_CIRC | DMA_CCR1_TCIE | DMA_CCR1_EN;
    NVIC_EnableIRQ(DMA1_Channel1_IRQn);
  } else DMA1_Channel1->CCR = DMA_CCR1_MINC |  DMA_CCR1_MSIZE_0 | DMA_CCR1_PSIZE_1 | DMA_CCR1_CIRC | DMA_CCR1_EN;

  while (ADC->CR2 & ADC_CR2_CAL) != 0) {}
//--------------------------------------------------------------------------------
  for (i = 0; i < ADC_MAX_CHN; i++)
  {
    if (i < 10 ) ADC1->SMPR2 |= ADC_SAMPLE_CONST << (i*3);

    if ( (i >= 10) && (i < ADC_MAX_CHN ) ) adc[adc_counter].port->SMPR1 |= ADC_SAMPLE_CONST << ( (i-10)*3);
  }
//--------------------------------------------------------------------------------
  for (i = 0; i < adc_chn; i++)
  {
    if (i < 6) adc[adc_counter].port->SQR3 |= adc_chl[i] << i*5;

    if ( (i >= 6) && (i < 12) )   adc[adc_counter].port->SQR2 |= adc_chl[i] << (i-6)*5;

    if ( (i >= 12) && (i < 16 ) ) adc[adc_counter].port->SQR1 |= adc_chl[i] << (i-12)*5;
  }

        adc[adc_counter].port->SQR1 |= ( (adc_chn - 1) << 20);  //
//--------------------------------------------------------------------------------
  adc[adc_counter].port->CR2 |= ADC_CR2_DMA | ADC_CR2_CONT;// | ADC_CR2_ADON;
  adc[adc_counter].port->CR1 |= ADC_CR1_SCAN;

  adc[adc_counter].port->CR2 |= ADC_CR2_ADON;

        if (flt) adc[adc_handler[adc_counter]].osComIndex = stAddTask(&adc_filter, stf_event);

        adc_counter++;

  return &adc[adc_counter-1].rbuf[0];
}
