
#define  ADC_MAX_CHN 18
#define  ADC_MAX_NUMBER 3

#include "adc_drv.h"
#include "adc_drv.c"

#ifndef RCC_APB2ENR_ADC2EN
void ADC1_IRQHandler(void)
{
  uint8_t adc_h;

  adc_h = adc_irq_id[0];

  adc[adc_h].sum += adc[adc_h].port->DR;

  if (++adc[adc_h].flt_cnt == (1<<adc[adc_h].flt_deep) ) stSetEvent(adc[adc_h].os_indx);
    else adc[adc_h].port->CR2 |= ADC_CR2_ADON;
}
#endif

#ifdef RCC_APB2ENR_ADC2EN
void ADC1_2_IRQHandler(void)
{
  uint8_t adc_h;

  if ((ADC1->SR & ADC_SR_EOC) == ADC_SR_EOC) adc_h = adc_irq_id[0]; else adc_h = adc_irq_id[1];

  adc[adc_h].sum += adc[adc_h].port->DR;

  if (++adc[adc_h].flt_cnt == (1<<adc[adc_h].flt_deep) ) stSetEvent(adc[adc_h].os_indx);
    else adc[adc_h].port->CR2 |= ADC_CR2_ADON;
}
#endif

#ifdef RCC_APB2ENR_ADC3EN
void ADC3_IRQHandler(void)
{
  uint8_t adc_h;

  adc_h = adc_irq_id[2];

  adc[adc_h].sum += adc[adc_h].port->DR;

  if (++adc[adc_h].flt_cnt == (1<<adc[adc_h].flt_deep) ) stSetEvent(adc[adc_h].os_indx);
    else adc[adc_h].port->CR2 |= ADC_CR2_ADON;
}
#endif

void DMA1_Channel1_IRQHandler(void)
{
  uint8_t adc_h;

  DMA1->IFCR = DMA_IFCR_CTCIF1;

  adc_h = adc_irq_id[0];

  adc[adc_h].handler();
}

static void adc_main(void)
{
  uint8_t adc_h;
  uint8_t   hdl;

  hdl = stGetTaskID();

  for (adc_h = 0; adc_h < ADC_NUMBER; adc_h++)
    if ( adc[adc_h].os_indx == hdl) break;

  adc[adc_h].buf[adc[adc_h].ch_cnt] = adc[adc_h].sum >> (adc[adc_h].flt_deep - adc[adc_h].flt_deci);

  adc[adc_h].sum = 0;
  adc[adc_h].flt_cnt = 0;

  if (++adc[adc_h].ch_cnt == adc[adc_h].chn) { adc[adc_h].ch_cnt = 0; if (adc[adc_h].handler) adc[adc_h].handler();}

  adc[adc_h].port->SQR3 = adc[adc_h].chls[adc[adc_h].ch_cnt] & 0x1f;

  adc[adc_h].port->CR2 |= ADC_CR2_ADON;
}


uint16_t* adc_add(ADC_TypeDef* port, uint8_t* chls, uint8_t chn, uint8_t flt_deep, uint8_t flt_deci, fptr handler, uint8_t smpl_const)
{
  int8_t i;

  IRQn_Type adc_irq;
  IRQn_Type adc_dma_irq;
  DMA_Channel_TypeDef* adc_dma;

  if (adc_counter == ADC_NUMBER) sysErrHandler(ser_adc_expnum);

  // check port
  for (i=0; i < adc_counter; i++)
    if (adc[i].port == port) sysErrHandler(ser_adc_cross);

  // check params
  if ( (chls == 0) || (chn == 0) || (chn > 16) || (flt_deep > 16) || (flt_deep == 0) || (flt_deci > flt_deep) ) sysErrHandler(ser_adc_param);

  adc[adc_counter].buf = malloc(chn*2);
  if ( adc[adc_counter].buf == 0 ) sysErrHandler(ser_heap_err);

  adc[adc_counter].port  = port;
  adc[adc_counter].chls  = chls;
  adc[adc_counter].chn   = chn;
  adc[adc_counter].flt_deep = flt_deep;
  adc[adc_counter].flt_deci  = flt_deci;
  adc[adc_counter].handler = handler;

  adc[adc_counter].flt_cnt  = 0;
  adc[adc_counter].ch_cnt  = 0;
  adc[adc_counter].sum = 0;

  if (port == ADC1)
  {
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;

    adc_irq_id[0] = adc_counter;

#ifndef RCC_APB2ENR_ADC2EN
    adc_irq = ADC1_IRQn;
#else
    adc_irq = ADC1_2_IRQn;
#endif

    adc_dma     = DMA1_Channel1;
    adc_dma_irq = DMA1_Channel1_IRQn;
  }

#ifdef RCC_APB2ENR_ADC2EN
  if (port == ADC2)
  {
    if (flt_deep == 0) sysErrHandler(ser_par_err);

    RCC->APB2ENR |= RCC_APB2ENR_ADC2EN;

    adc_irq_id[1] = adc_counter;

    adc_irq = ADC1_2_IRQn;
  }
#endif

#ifdef RCC_APB2ENR_ADC3EN
  if (port == ADC3)
  {
    if (flt_deep == 0) sysErrHandler(ser_par_err);

    RCC->APB2ENR |= RCC_APB2ENR_ADC3EN;

    adc_irq_id[2] = adc_counter;

    adc_irq = ADC3_IRQn;
  }
#endif

  port->CR2 = ADC_CR2_TSVREFE ;
  port->CR2 |= ADC_CR2_CAL | ADC_CR2_ADON; // start calibration

  while ( (port->CR2 & ADC_CR2_CAL) != 0) {}

  for (i = 0; i < ADC_MAX_CHN; i++)
  {
    if (i < 10 ) port->SMPR2 |= smpl_const << (i*3);

    if ( (i >= 10) && (i < ADC_MAX_CHN ) ) port->SMPR1 |= smpl_const << ( (i-10)*3);
  }

  if (flt_deep) // adc filter
  {
    port->SQR1 = 0;
    port->CR1 = ADC_CR1_EOCIE;
    NVIC_EnableIRQ(adc_irq);

    port->SQR3 = chls[0] & 0x1f;
    port->CR2 |= ADC_CR2_ADON;

    adc[adc_counter].os_indx = stAddTask(&adc_main, stf_event);
  } else // adc simple
  {
    RCC->AHBENR  |= RCC_AHBENR_DMA1EN;

    for (i = 0; i < chn; i++)
    {
      if (i < 6) port->SQR3 |= chls[i] << i*5;

      if ( (i >= 6) && (i < 12) )   port->SQR2 |= chls[i] << (i-6)*5;

      if ( (i >= 12) && (i < 16 ) ) port->SQR1 |= chls[i] << (i-12)*5;
    }

    port->SQR1 |= ( (chn - 1) << 20);  // chan num

    if (adc_dma->CCR != 0)  sysErrHandler(ser_dma_cross);

    adc_dma->CPAR  = (uint32_t) &port->DR;
    adc_dma->CMAR  = (uint32_t) adc[adc_counter].buf;
    adc_dma->CNDTR = chn ;

    if (handler)
    {
      adc_dma->CCR = DMA_CCR1_MINC |  DMA_CCR1_MSIZE_0 | DMA_CCR1_PSIZE_1 | DMA_CCR1_CIRC | DMA_CCR1_TCIE | DMA_CCR1_EN;
      NVIC_EnableIRQ(adc_dma_irq);
    } else adc_dma->CCR = DMA_CCR1_MINC |  DMA_CCR1_MSIZE_0 | DMA_CCR1_PSIZE_1 | DMA_CCR1_CIRC | DMA_CCR1_EN;

    port->CR2 |= ADC_CR2_DMA | ADC_CR2_CONT;
    port->CR1 |= ADC_CR1_SCAN;

    port->CR2 |= ADC_CR2_ADON;
  }

  adc_counter++;

  return adc[adc_counter-1].buf;
}
