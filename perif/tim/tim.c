//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
#include "system.h"
#include "tim.h"
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// F0 series
#if defined(STM32F030) || defined(STM32F051) || \
    defined(STM32F031) || defined(STM32F072) || \
    defined(STM32F042)
  #include "drv\tim_drv_f0xx.c"
#endif

//-----------------------------------------------------------------------------
// F1 series

#if defined(STM32F10X_LD) || defined(STM32F10X_LD_VL) || \
    defined(STM32F10X_MD) || defined(STM32F10X_MD_VL) || \
    defined(STM32F10X_HD) || defined(STM32F10X_HD_VL) || \
    defined(STM32F10X_XL) || defined (STM32F10X_CL)
  #include "drv\tim_drv_f10x.c"
#endif

// STM32F303x6, STM32F303x8
// STM32F303xB, STM32F303xC
#if defined(STM32F303x6) || defined(STM32F303x8)  || \
    defined(STM32F303xB) ||  defined(STM32F303xC) || \
    defined(STM32F334)
  #include "drv\tim_drv_f303xc.c"
#endif
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
uint8_t tim_get_indx(TIM_TypeDef* tim_l)
{
  uint8_t tim_h, i;

  tim_h = 0xff;

  for (i = 0; i < TIM_COUNT; i++)
    if (tim_list[i].tim == tim_l)
      {
        tim_h = i;
        break;
      }
  return tim_h;
}

void tim_rcc_en(TIM_TypeDef* tim_l)
{
  uint8_t tim_h;
  __IO uint32_t* rcc_reg;

  tim_h = tim_get_indx(tim_l);

  if (tim_h == 0xff) return;

  rcc_reg = (__IO uint32_t*) tim_list[tim_h].rcc_en_reg;

  *rcc_reg |= tim_list[tim_h].rcc_en_mask;
}

uint32_t tim_apb_freq(TIM_TypeDef* tim_l)
{
  uint8_t tim_h;

  tim_h = tim_get_indx(tim_l);

  if (tim_h == 0xff) return 0xffffffff;
    else return tim_list[tim_h].apb_freq;
}


void tim_setup_freq(TIM_TypeDef* tim_l, uint32_t freq, uint8_t slope)
{
  uint8_t tim_h;
  uint32_t temp;
  uint8_t ofs;

  tim_h = tim_get_indx(tim_l);

  if (tim_h == 0xff) return;

  slope &= 1;

  ofs = 1 - slope;

  if (slope)
    tim_list[tim_h].tim->CR1 |= TIM_CR1_ARPE | TIM_CR1_URS | (TIM_CR1_CMS_0*1);
  else
    tim_list[tim_h].tim->CR1 = TIM_CR1_ARPE | TIM_CR1_URS | (TIM_CR1_CMS_0*0);

  slope++;

  freq = freq*slope;

  if (freq == 0) return;

  temp = tim_list[tim_h].apb_freq/freq;

  if (temp > 0x10000)
  {
    tim_list[tim_h].tim->PSC = temp/0x10000;
    tim_list[tim_h].tim->ARR = tim_list[tim_h].apb_freq/(TIM3->PSC+1)/freq - ofs;
  }
  else
  {
    tim_list[tim_h].tim->PSC = 0;
    tim_list[tim_h].tim->ARR = temp - ofs;
  }
}


void tim_en(TIM_TypeDef* tim)
{
  tim->CR1 |= TIM_CR1_CEN;
}

void tim_dis(TIM_TypeDef* tim)
{
  tim->CR1 &= ~TIM_CR1_CEN;
}

// ch:  0=dis, 1=non inverse, 2=inverse
void tim_setup_pwm(TIM_TypeDef* tim, uint8_t ch1, uint8_t ch2, uint8_t ch3, uint8_t ch4)
{
        uint16_t cmr1, cmr2, cer;

        cmr1 = 0;
        cmr2 = 0;
        cer  = 0;

        if (ch1 == 1) {cmr1 |= TIM_CCMR1_OC1PE | (TIM_CCMR1_OC1M_0*6); cer |= TIM_CCER_CC1E | (TIM_CCER_CC1P*0);} else
        if (ch1 == 2) {cmr1 |= TIM_CCMR1_OC1PE | (TIM_CCMR1_OC1M_0*6); cer |= TIM_CCER_CC1E | (TIM_CCER_CC1P*1);}

        if (ch2 == 1) {cmr1 |= TIM_CCMR1_OC2PE | (TIM_CCMR1_OC2M_0*6); cer |= TIM_CCER_CC2E | (TIM_CCER_CC2P*0);} else
        if (ch2 == 2) {cmr1 |= TIM_CCMR1_OC2PE | (TIM_CCMR1_OC2M_0*6); cer |= TIM_CCER_CC2E | (TIM_CCER_CC2P*1);}

        if (ch3 == 1) {cmr2 |= TIM_CCMR2_OC3PE | (TIM_CCMR2_OC3M_0*6); cer |= TIM_CCER_CC3E | (TIM_CCER_CC3P*0);} else
        if (ch3 == 2) {cmr2 |= TIM_CCMR2_OC3PE | (TIM_CCMR2_OC3M_0*6); cer |= TIM_CCER_CC3E | (TIM_CCER_CC3P*1);}

        if (ch4 == 1) {cmr2 |= TIM_CCMR2_OC4PE | (TIM_CCMR2_OC4M_0*6); cer |= TIM_CCER_CC4E | (TIM_CCER_CC4P*0);} else
        if (ch4 == 2) {cmr2 |= TIM_CCMR2_OC4PE | (TIM_CCMR2_OC4M_0*6); cer |= TIM_CCER_CC4E | (TIM_CCER_CC4P*1);}

        tim->CCMR1 = cmr1;
        tim->CCMR2 = cmr2;
        tim->CCER  = cer;

        tim->BDTR |= TIM_BDTR_MOE;
}

void tim_pwm1_set(TIM_TypeDef* tim, uint16_t pwm)
{
  tim->CCR1 = pwm;
}

void tim_pwm2_set(TIM_TypeDef* tim, uint16_t pwm)
{
  tim->CCR2 = pwm;
}

void tim_pwm3_set(TIM_TypeDef* tim, uint16_t pwm)
{
  tim->CCR3 = pwm;
}

void tim_pwm4_set(TIM_TypeDef* tim, uint16_t pwm)
{
  tim->CCR4 = pwm;
}


void tim_pwm_set(TIM_TypeDef* tim, uint8_t ch, uint16_t pwm)
{
  if (ch == 0) tim->CCR1 = pwm; else
  if (ch == 1) tim->CCR2 = pwm; else
  if (ch == 2) tim->CCR3 = pwm; else
  if (ch == 3) tim->CCR4 = pwm;
}

uint16_t tim_ARR(TIM_TypeDef* tim)
{
  return tim->ARR;
}

// 0 - update int
// 1..4 - comp int
void tim_int_en(TIM_TypeDef* tim, uint8_t int_t)
{
  if (int_t == 0) tim->DIER |= TIM_DIER_UIE ; else
  if (int_t == 1) tim->DIER |= TIM_DIER_CC1IE; else
  if (int_t == 2) tim->DIER |= TIM_DIER_CC2IE; else
  if (int_t == 3) tim->DIER |= TIM_DIER_CC3IE; else
  if (int_t == 4) tim->DIER |= TIM_DIER_CC4IE;
}

// 0 - update int
// 1..4 - comp int
void tim_int_dis(TIM_TypeDef* tim, uint8_t int_t)
{
  if (int_t == 0) tim->DIER &= ~TIM_DIER_UIE ; else
  if (int_t == 1) tim->DIER &= ~TIM_DIER_CC1IE; else
  if (int_t == 2) tim->DIER &= ~TIM_DIER_CC2IE; else
  if (int_t == 3) tim->DIER &= ~TIM_DIER_CC3IE; else
  if (int_t == 4) tim->DIER &= ~TIM_DIER_CC4IE;
}

// 0 - update int
// 1..4 - comp int
void tim_int_clr(TIM_TypeDef* tim, uint8_t int_t)
{
  if (int_t == 0) tim->SR &= ~TIM_SR_UIF   ; else
  if (int_t == 1) tim->SR &= ~TIM_SR_CC1IF ; else
  if (int_t == 2) tim->SR &= ~TIM_SR_CC2IF ; else
  if (int_t == 3) tim->SR &= ~TIM_SR_CC3IF ; else
  if (int_t == 4) tim->SR &= ~TIM_SR_CC4IF ;
}

uint8_t tim_int_flag(TIM_TypeDef* tim, uint8_t int_t)
{
  if (int_t == 0) return (tim->SR & TIM_SR_UIF)   != 0; else
  if (int_t == 1) return (tim->SR & TIM_SR_CC1IF) != 0; else
  if (int_t == 2) return (tim->SR & TIM_SR_CC2IF) != 0; else
  if (int_t == 3) return (tim->SR & TIM_SR_CC3IF) != 0; else
  if (int_t == 4) return (tim->SR & TIM_SR_CC4IF) != 0;

        return 0;
}

void tim_setup_arr(TIM_TypeDef* tim_l, uint16_t arr, uint16_t psc, uint8_t slope)
{
  uint8_t tim_h;

  tim_h = tim_get_indx(tim_l);

  if (tim_h == 0xff) return;

  slope &= 1;

  if (slope)
    tim_list[tim_h].tim->CR1 |= TIM_CR1_ARPE | TIM_CR1_URS | (TIM_CR1_CMS_0*1);
  else
    tim_list[tim_h].tim->CR1 = TIM_CR1_ARPE | TIM_CR1_URS | (TIM_CR1_CMS_0*0);

  tim_list[tim_h].tim->PSC = psc;
  tim_list[tim_h].tim->ARR = arr;
}

