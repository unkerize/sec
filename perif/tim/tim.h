//-------------------------------------------------------------------------
#include "system.h"
//-----------------------------------------------------------------------------

#ifndef _HK_TIM_H
#define _HK_TIM_H

typedef struct {
  TIM_TypeDef*   tim;
  __IO uint32_t* rcc_en_reg;
  uint32_t       rcc_en_mask;
  uint32_t       apb_freq;
} TIM_DRV_T;


uint8_t tim_get_indx(TIM_TypeDef* tim_l);

void tim_rcc_en(TIM_TypeDef* tim_l);
uint32_t tim_apb_freq(TIM_TypeDef* tim_l);
void tim_setup_freq(TIM_TypeDef* tim_l, uint32_t freq, uint8_t slope);

void tim_en(TIM_TypeDef* tim);
void tim_dis(TIM_TypeDef* tim);
void tim_setup_pwm(TIM_TypeDef* tim, uint8_t ch1, uint8_t ch2, uint8_t ch3, uint8_t ch4);

void tim_pwm1_set(TIM_TypeDef* tim, uint16_t pwm);
void tim_pwm2_set(TIM_TypeDef* tim, uint16_t pwm);
void tim_pwm3_set(TIM_TypeDef* tim, uint16_t pwm);
void tim_pwm4_set(TIM_TypeDef* tim, uint16_t pwm);
void tim_pwm_set(TIM_TypeDef* tim, uint8_t ch, uint16_t pwm);

uint16_t tim_ARR(TIM_TypeDef* tim);

// 0 - update int
// 1..4 - comp int
void tim_int_en(TIM_TypeDef* tim, uint8_t int_t);
void tim_int_dis(TIM_TypeDef* tim, uint8_t int_t);
void tim_int_clr(TIM_TypeDef* tim, uint8_t int_t);
uint8_t tim_int_flag(TIM_TypeDef* tim, uint8_t int_t);

void tim_setup_arr(TIM_TypeDef* tim_l, uint16_t arr, uint16_t psc, uint8_t slope);

// timWGM_OCMASK :
// 0,1,2,3 bits = OC en
// 4..7         = WGM, 0 = single slope, 1 = dual slope
//void timPwmCFG(TIM_TypeDef* timInst, uint16_t timPSC, uint16_t timARR, uint32_t timWGM_OCMASK);
// l_chan = 1..4
//void timPwmSET(TIM_TypeDef* timInst,uint8_t l_chan,uint16_t l_pwm);
//-------------------------------------------------------------------------
#endif  // _HK_TIM_H




