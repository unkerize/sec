#define  ADC_MAX_CHN 18

#pragma pack(push,1)
typedef struct {
  uint8_t*     chls;
  uint16_t*    buf;
  uint8_t      chn;

  uint8_t   flt_deep;
  uint8_t   flt_deci;

  uint8_t   os_indx;
  fptr      handler;

  uint16_t  flt_cnt;
  uint8_t   ch_cnt;
  uint32_t  sum;

} adc_type;
#pragma pack(pop)

adc_type adc;
int8_t   adc_counter = 0;

#ifndef RCC_APB2ENR_ADC2EN
void ADC1_IRQHandler(void)
{
  adc.sum += ADC1->DR;

  if (++adc.flt_cnt == (1<<adc.flt_deep) ) stSetEvent(adc.os_indx);
    else ADC1->CR = ADC_CR_ADSTART ;
}
#endif

#ifdef RCC_APB2ENR_ADC2EN
void ADC1_2_IRQHandler(void)
{
  adc.sum += ADC1->DR;

  if (++adc.flt_cnt == (1<<adc.flt_deep) ) stSetEvent(adc.os_indx);
    else ADC1->CR = ADC_CR_ADSTART ;
}
#endif

void DMA1_Channel1_IRQHandler(void)
{
  DMA1->IFCR = DMA_IFCR_CTCIF1;

  adc.handler();
}

static void adc_main(void)
{
  adc.buf[adc.ch_cnt] = adc.sum >> (adc.flt_deep - adc.flt_deci);

  adc.sum = 0;
  adc.flt_cnt = 0;

  if (++adc.ch_cnt == adc.chn) { adc.ch_cnt = 0; if (adc.handler) adc.handler();}

  ADC1->CHSELR = 1<<adc.chls[adc.ch_cnt];

  ADC1->CR = ADC_CR_ADSTART ;
}


uint16_t* adc_add(ADC_TypeDef* port,  uint8_t* chls, uint8_t chn, uint8_t flt_deep, uint8_t flt_deci, fptr handler, uint8_t smpl_const)
{
  int8_t i;

  IRQn_Type adc_irq;

  // check params
  if ( (chls == 0) || (chn == 0) || (chn > 16) || (flt_deep > 16) || (flt_deep == 0) || (flt_deci > flt_deep) ) sysErrHandler(ser_adc_param);

  adc.buf = malloc(chn*2);
  if ( adc.buf == 0 ) sysErrHandler(ser_heap_err);

  adc.chls  = chls;
  adc.chn   = chn;
  adc.flt_deep = flt_deep;
  adc.flt_deci  = flt_deci;
  adc.handler = handler;

  adc.flt_cnt  = 0;
  adc.ch_cnt  = 0;
  adc.sum = 0;

  RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;



#if   defined (STM32F051)
        adc_irq = ADC1_COMP_IRQn;
#elif defined (STM32F072)
        adc_irq = ADC1_COMP_IRQn;
#elif defined (STM32F030)
        adc_irq = ADC1_IRQn;
#elif defined (STM32F031)
        adc_irq = ADC1_IRQn;
#elif defined (STM32F042)
        adc_irq = ADC1_IRQn;
#endif

  // Calibration
  ADC1->CR = ADC_CR_ADCAL ;
  while ( (ADC1->CR & ADC_CR_ADCAL) != 0)    {}

  // Enable ADC
  ADC1->CR = ADC_CR_ADEN;
  while ( (ADC1->ISR & ADC_ISR_ADRDY ) == 0) {}

  // Set sample times
  ADC1->SMPR = smpl_const;

  if (flt_deep) // adc filter
  {
    ADC1->CHSELR = 1<<chls[0];

    ADC1->IER = ADC_IER_EOCIE;

    ADC1->CFGR1 = ADC_CFGR1_DISCEN;

    NVIC_EnableIRQ(adc_irq);

    adc.os_indx = stAddTask(&adc_main, stf_event);

    ADC1->CR = ADC_CR_ADSTART ;
  } else // adc simple
  {
    RCC->AHBENR  |= RCC_AHBENR_DMA1EN;

    for (i = 0; i < chn; i++)
      ADC1->CHSELR |= 1<<chls[i];

    if (DMA1_Channel1->CCR != 0)  sysErrHandler(ser_dma_cross);

    DMA1_Channel1->CPAR  = (uint32_t) &(ADC1->DR);
    DMA1_Channel1->CMAR  = (uint32_t) adc.buf;
    DMA1_Channel1->CNDTR = chn ;

    if (handler)
    {
      DMA1_Channel1->CCR = DMA_CCR_MINC |  DMA_CCR_MSIZE_0 | DMA_CCR_PSIZE_1 | DMA_CCR_CIRC | DMA_CCR_TCIE | DMA_CCR_EN;
      NVIC_EnableIRQ(DMA1_Channel1_IRQn);
    } else DMA1_Channel1->CCR = DMA_CCR_MINC |  DMA_CCR_MSIZE_0 | DMA_CCR_PSIZE_1 | DMA_CCR_CIRC | DMA_CCR_EN;

    ADC1->CFGR1 = ADC_CFGR1_DMAEN | ADC_CFGR1_DMACFG | ADC_CFGR1_CONT;
    ADC1->CR = ADC_CR_ADSTART ;
  }
  return adc.buf;
}
