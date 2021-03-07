
#define  ADC_MAX_CHN    18
#define  ADC_MAX_NUMBER 4

#include "adc_drv.h"
#include "adc_drv.c"

void ADC1_2_IRQHandler(void)
{

  uint8_t adc_h;

  if ((ADC1->ISR & ADC_ISR_EOC) == ADC_ISR_EOC) adc_h = adc_irq_id[0];
   else adc_h = adc_irq_id[1];

  adc[adc_h].sum += adc[adc_h].port->DR;

  if (++adc[adc_h].flt_cnt == (1<<adc[adc_h].flt_deep) ) stSetEvent(adc[adc_h].os_indx);
    else adc[adc_h].port->CR |= ADC_CR_ADSTART;
}


#ifdef RCC_AHBENR_ADC34EN
void ADC3_IRQHandler(void)
{
  uint8_t adc_h;

  adc_h = adc_irq_id[2];

  adc[adc_h].sum += adc[adc_h].port->DR;

  if (++adc[adc_h].flt_cnt == (1<<adc[adc_h].flt_deep) ) stSetEvent(adc[adc_h].os_indx);
    else adc[adc_h].port->CR |= ADC_CR_ADSTART;
}

void ADC4_IRQHandler(void)
{
  uint8_t adc_h;

  adc_h = adc_irq_id[3];

  adc[adc_h].sum += adc[adc_h].port->DR;

  if (++adc[adc_h].flt_cnt == (1<<adc[adc_h].flt_deep) ) stSetEvent(adc[adc_h].os_indx);
    else adc[adc_h].port->CR |= ADC_CR_ADSTART;
}
#endif

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

  adc[adc_h].port->SQR1 = ( (adc[adc_h].chls[adc[adc_h].ch_cnt]+1) & 0x1f) * ADC_SQR1_SQ1_0 ;

  adc[adc_h].port->CR |= ADC_CR_ADSTART;
}

uint16_t* adc_add(ADC_TypeDef* port, uint8_t* chls, uint8_t chn, uint8_t flt_deep, uint8_t flt_deci, fptr handler, uint8_t smpl_const)
{
  int8_t i;

  IRQn_Type adc_irq;

  if (adc_counter == ADC_NUMBER) sysErrHandler(ser_adc_expnum);

  // check port
  for (i=0; i < adc_counter; i++)
    if (adc[i].port == port) sysErrHandler(ser_adc_cross);

  // check params
  if ( (chls == 0) || (chn == 0) || (chn > 16) || (flt_deep == 0) || (flt_deep > 16) || (flt_deci > flt_deep) ) sysErrHandler(ser_adc_param);

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
    RCC->AHBENR |= RCC_AHBENR_ADC12EN;
    adc_irq_id[0] = adc_counter;
    adc_irq = ADC1_2_IRQn;
  }

  if (port == ADC2)
  {
    RCC->AHBENR |= RCC_AHBENR_ADC12EN;
    adc_irq_id[1] = adc_counter;
    adc_irq = ADC1_2_IRQn;
  }


#ifdef RCC_AHBENR_ADC34EN
  if (port == ADC3)
  {
    RCC->AHBENR |= RCC_AHBENR_ADC34EN;
    adc_irq_id[2] = adc_counter;
    adc_irq = ADC3_IRQn;
  }

  if (port == ADC4)
  {
    RCC->AHBENR |= RCC_AHBENR_ADC34EN;
    adc_irq_id[3] = adc_counter;
    adc_irq = ADC4_IRQn;
  }
#endif

  port->CR = ADC_CR_ADVREGEN_0*0  ; // enable ADC voltage reg
  port->CR = ADC_CR_ADVREGEN_0*1  ;

//  syncDelay(10);                    // wait for startup
  usecDelay(10);

  port->CR |= ADC_CR_ADCAL;

  while ( (port->CR & ADC_CR_ADCAL) != 0) {} // wait for calibration

//  syncDelay(20);                             // wait min 4 adc_sck
  usecDelay(20);

  port->CR |= ADC_CR_ADEN;

  while ( (port->ISR & ADC_ISR_ADRD) != ADC_ISR_ADRD) {} // wait for ready

  for (i = 0; i < ADC_MAX_CHN; i++)
  {
    if (i < 9 ) port->SMPR1 |= smpl_const << (3+i*3);

    if ( (i >= 9) && (i < ADC_MAX_CHN ) ) port->SMPR2 |= smpl_const << ( (i-9)*3);
  }

  port->SQR1 = ( (chls[0]+1) & 0x1f) * ADC_SQR1_SQ1_0;

  port->IER = ADC_IER_EOC;

  NVIC_EnableIRQ(adc_irq);

  adc[adc_counter].os_indx = stAddTask(&adc_main, stf_event);
  adc_counter++;

  port->CR |= ADC_CR_ADSTART;

  return adc[adc_counter-1].buf;
}
