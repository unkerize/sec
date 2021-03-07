
#ifndef _HK_DS_H
#define _HK_DS_H

#include "system.h"
#include "ds_cfg.h"

#if ! ( (DS_TIMER >= 1) && (DS_TIMER <= 5) || (DS_TIMER == 8) )
#error "DS: Timer incorrect"
#endif

#if DS_TIMER == 1
  #define DS_TIM              TIM1
  #define DS_TIM_RCC_EN()     RCC->APB2ENR |= RCC_APB2ENR_TIM1EN
  #define DS_TIM_IRQ          TIM1_CC_IRQn
  #define DS_TIM_IRQ_HANDLER  TIM1_CC_IRQHandler
  #define DS_BASE_FREQ (F_APB2)
#endif

#if DS_TIMER == 2
  #define DS_TIM              TIM2
  #define DS_TIM_RCC_EN()     RCC->APB1ENR |= RCC_APB1ENR_TIM2EN
  #define DS_TIM_IRQ          TIM2_IRQn
  #define DS_TIM_IRQ_HANDLER  TIM2_IRQHandler
  #define DS_BASE_FREQ (F_APB1)
#endif

#if DS_TIMER == 3
  #define DS_TIM              TIM3
  #define DS_TIM_RCC_EN()     RCC->APB1ENR |= RCC_APB1ENR_TIM3EN
  #define DS_TIM_IRQ          TIM3_IRQn
  #define DS_TIM_IRQ_HANDLER  TIM3_IRQHandler
  #define DS_BASE_FREQ (F_APB1)
#endif

#if DS_TIMER == 4
  #define DS_TIM              TIM4
  #define DS_TIM_RCC_EN()     RCC->APB1ENR |= RCC_APB1ENR_TIM4EN
  #define DS_TIM_IRQ          TIM4_IRQn
  #define DS_TIM_IRQ_HANDLER  TIM4_IRQHandler
  #define DS_BASE_FREQ (F_APB1)
#endif

#if DS_TIMER == 5
  #define DS_TIM              TIM5
  #define DS_TIM_RCC_EN()     RCC->APB1ENR |= RCC_APB1ENR_TIM5EN
  #define DS_TIM_IRQ          TIM5_IRQn
  #define DS_TIM_IRQ_HANDLER  TIM5_IRQHandler
  #define DS_BASE_FREQ (F_APB1)
#endif


#if DS_CHAN == 1
 #define DS_W_REG   CCR1
 #define DS_R_REG   CCR2
 #define DS_IRQ_REG CCR3

 #define DS_SR_MASK TIM_SR_CC3IF
 #define DS_IE_MASK TIM_DIER_CC3IE
 #define DS_CMR_REG CCMR1
 #define DS_CMR_MASK (TIM_CCMR1_CC1S_0*0 | TIM_CCMR1_OC1M_0*6  | TIM_CCMR1_CC2S_0*2)
 #define DS_CE_MASK_LONG  (TIM_CCER_CC1E | TIM_CCER_CC1P) | (TIM_CCER_CC2E | (TIM_CCER_CC2P*1) )
 #define DS_CE_MASK_SHORT (TIM_CCER_CC1E | TIM_CCER_CC1P) | (TIM_CCER_CC2E | (TIM_CCER_CC2P*0) )
#endif

#if DS_CHAN == 2
 #define DS_W_REG CCR2
 #define DS_R_REG CCR1
 #define DS_IRQ_REG CCR3

 #define DS_SR_MASK TIM_SR_CC3IF
 #define DS_IE_MASK TIM_DIER_CC3IE
 #define DS_CMR_REG CCMR1
 #define DS_CMR_MASK (TIM_CCMR1_CC2S_0*0 | TIM_CCMR1_OC2M_0*6  | TIM_CCMR1_CC1S_0*2)
 #define DS_CE_MASK_LONG  (TIM_CCER_CC2E | TIM_CCER_CC2P) | (TIM_CCER_CC1E | (TIM_CCER_CC1P*1) )
 #define DS_CE_MASK_SHORT (TIM_CCER_CC2E | TIM_CCER_CC2P) | (TIM_CCER_CC1E | (TIM_CCER_CC1P*0) )
#endif


#if DS_CHAN == 3
 #define DS_W_REG CCR3
 #define DS_R_REG CCR4
 #define DS_IRQ_REG CCR1

 #define DS_SR_MASK TIM_SR_CC1IF
 #define DS_IE_MASK TIM_DIER_CC1IE
 #define DS_CMR_REG     CCMR2
 #define DS_CMR_MASK (TIM_CCMR1_CC1S_0*0 | TIM_CCMR1_OC1M_0*6  | TIM_CCMR1_CC2S_0*2)
 #define DS_CE_MASK_LONG  (TIM_CCER_CC3E | TIM_CCER_CC3P) | (TIM_CCER_CC4E | (TIM_CCER_CC4P*1) )
 #define DS_CE_MASK_SHORT (TIM_CCER_CC3E | TIM_CCER_CC3P) | (TIM_CCER_CC4E | (TIM_CCER_CC4P*0) )
#endif

#if DS_CHAN == 4
 #define DS_W_REG CCR4
 #define DS_R_REG CCR3
 #define DS_IRQ_REG CCR1

 #define DS_SR_MASK TIM_SR_CC1IF
 #define DS_IE_MASK TIM_DIER_CC1IE
 #define DS_CMR_REG     CCMR2
 #define DS_CMR_MASK (TIM_CCMR1_CC2S_0*0 | TIM_CCMR1_OC2M_0*6  | TIM_CCMR1_CC1S_0*2)
 #define DS_CE_MASK_LONG  (TIM_CCER_CC4E | TIM_CCER_CC4P) | (TIM_CCER_CC3E | (TIM_CCER_CC3P*1) )
 #define DS_CE_MASK_SHORT (TIM_CCER_CC4E | TIM_CCER_CC4P) | (TIM_CCER_CC3E | (TIM_CCER_CC3P*0) )
#endif


#define DS_NOT_PRESENT      (-200*16)
#define DS_CRC_ERROR        (-201*16)


#define DS_CPUSEC (DS_BASE_FREQ/1000000)

#define DS_RESET_PULSE  500
#define DS_WR0_PULSE    90
#define DS_WR1_PULSE    8
#define DS_RD_PULSE     6
#define DS_RD_ACK_PULSE     (DS_RD_PULSE+5)
#define DS_TIME_SLOT    400//200

#define DS_CMD_SKIP_ROM 0xCC
#define DS_CMD_READ_POWER_STATUS  0xB4
#define DS_CMD_READ_SCRATCH_PAD   0xBE
#define DS_CMD_START_CONV         0x44

int16_t ds_read_temp(void);
void ds_init(fptr l_h);

#endif
