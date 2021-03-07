//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
// clkInit
//-------------------------------------------------------------------------
void clkInit(void)
{

#if defined(_PLL_MUL)
    RCC->CFGR2 = (uint32_t) _PLL_DIV - 1;
#endif


#if _CLK_SOURCE == _CLK_HSE
    RCC->CR |= RCC_CR_HSEON;

    while ((RCC->CR & RCC_CR_HSERDY) != RCC_CR_HSERDY) {}; // wait for stable
#endif

#ifndef _PLL_MUL
    RCC->CFGR |= (uint32_t) ( ((2   -2)      << 18) | \
                             ((_CLK_SOURCE-1)      << 16) | \
                             ((_CLK_APB2_MASK) << 13) | \
                             ((_CLK_APB1_MASK) << 8 ) | \
                             ((_CLK_AHB_MASK)  << 4 ) );
#else
    RCC->CFGR |= (uint32_t) ( ((_PLL_MUL   -2)      << 18) | \
                             ((_CLK_SOURCE-1)      << 16) | \
                             ((_CLK_APB2_MASK) << 13) | \
                             ((_CLK_APB1_MASK) << 8 ) | \
                             ((_CLK_AHB_MASK)  << 4 ) );
#endif

#if defined(_PLL_MUL)
    RCC->CR |= RCC_CR_PLLON;
    while ((RCC->CR & RCC_CR_PLLRDY) != RCC_CR_PLLRDY) {};  // wait for stable
#endif

#if   !defined(_PLL_MUL) && _CLK_SOURCE == _CLK_HSI
#elif !defined(_PLL_MUL) && _CLK_SOURCE == _CLK_HSE
      RCC->CFGR |=  RCC_CFGR_SW_HSE;
#elif defined(_PLL_MUL) && _CLK_SOURCE == _CLK_HSE
      RCC->CFGR |=  RCC_CFGR_SW_PLL;
#endif
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void gpioCFG(GPIO_TypeDef* port,uint16_t pin, uint32_t pincfg)
{
  uint8_t dir;
  uint8_t outtype;
  uint8_t intype;
  uint8_t outspeed;
  uint16_t alter;
  uint8_t alterF;

  dir = pincfg  &  0x3;

  if(dir == gpd_analog)
  {
      port->MODER |= 0x3 << (pin*2);
      return;
  }

  intype   = pincfg  & (0x3 << 4);
  outtype  = pincfg  & (0x3 << 2);
  outspeed = pincfg  & (0x3 << 6);

  port->MODER &= ~(0x3 << (pin*2));

  port->PUPDR &=  ~(0x3 << (pin*2));
  if (intype == gpit_pullup) port->PUPDR |=  0x1 << (pin*2); else
  if (intype == gpit_pulldn) port->PUPDR |=  0x2 << (pin*2);

  if (outtype == gpot_pushpull) port->OTYPER &= ~(1<<pin); else
  if (outtype == gpot_opendrain) port->OTYPER |= 1<<pin;

  port->OSPEEDR &= ~(0x3 << (pin*2));
  switch (outspeed)
  {
      case  gpos_lo:
          port->OSPEEDR |= 0x0 << (pin*2);
          break;
      case  gpos_me:
          port->OSPEEDR |= 0x1 << (pin*2);
          break;
      case  gpos_hi:
          port->OSPEEDR |= 0x3 << (pin*2);
          break;
      default:
          port->OSPEEDR |= 0x3 << (pin*2);
          break;
  }

  if(dir == gpd_input) return;

  if(dir == gpd_output) {port->MODER |= 0x1 << (pin*2);return;}

  alter = pincfg  & (0x3 << 8);

  alterF = pincfg >> 12;

  if (alter == gpaf_alter)
  {
      port->MODER |= 0x2 << (pin*2);

      port->AFR[pin/8] &= ~(0xf<<((pin%8) *4));
      port->AFR[pin/8] |= alterF<<((pin%8)*4);
  }
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void gpioEN_RCC(uint32_t rccMask)
{
  if (rccMask & gpioRCC_A) RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
  if (rccMask & gpioRCC_B) RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
  if (rccMask & gpioRCC_C) RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
  if (rccMask & gpioRCC_D) RCC->AHBENR |= RCC_AHBENR_GPIODEN;
  if (rccMask & gpioRCC_E) RCC->AHBENR |= RCC_AHBENR_GPIOEEN;
  if (rccMask & gpioRCC_F) RCC->AHBENR |= RCC_AHBENR_GPIOFEN;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
// timWGM_OCMASK :
// 0,1,2,3 bits = OC en
// 4..7         = WGM, 0 = single slope, 1 = dual slope
void timPwmCFG(TIM_TypeDef* timInst, uint16_t timPSC, uint16_t timARR, uint32_t timWGM_OCMASK)
{
  if (timInst == TIM1) RCC->APB2ENR |= RCC_APB2ENR_TIM1EN; else

  if (timInst == TIM2) RCC->APB1ENR |= RCC_APB1ENR_TIM2EN; else
  if (timInst == TIM3) RCC->APB1ENR |= RCC_APB1ENR_TIM3EN; else
#ifdef RCC_APB1ENR_TIM5EN
  if (timInst == TIM5) RCC->APB1ENR |= RCC_APB1ENR_TIM5EN; else
#endif
  if (timInst == TIM6) RCC->APB1ENR |= RCC_APB1ENR_TIM6EN; else
  if (timInst == TIM7) RCC->APB1ENR |= RCC_APB1ENR_TIM7EN; else
#ifdef RCC_APB1ENR_TIM12EN
  if (timInst == TIM12) RCC->APB1ENR |= RCC_APB1ENR_TIM12EN; else
#endif
#ifdef RCC_APB1ENR_TIM13EN
  if (timInst == TIM13) RCC->APB1ENR |= RCC_APB1ENR_TIM13EN; else
#endif
#ifdef RCC_APB1ENR_TIM14EN
  if (timInst == TIM14) RCC->APB1ENR |= RCC_APB1ENR_TIM14EN; else
#endif

  if (timInst == TIM15) RCC->APB2ENR |= RCC_APB2ENR_TIM15EN; else
  if (timInst == TIM16) RCC->APB2ENR |= RCC_APB2ENR_TIM16EN; else
  if (timInst == TIM17) RCC->APB2ENR |= RCC_APB2ENR_TIM17EN;


  timInst->CNT = 0;
  timInst->PSC = 0;
  timInst->ARR = timARR;

  timInst->CCMR1 |= TIM_CCMR1_OC1PE | (TIM_CCMR1_OC1M_1|TIM_CCMR1_OC1M_2 ) | \
                    TIM_CCMR1_OC2PE | (TIM_CCMR1_OC2M_1|TIM_CCMR1_OC2M_2 );

  timInst->CCMR2 |= TIM_CCMR2_OC3PE | (TIM_CCMR2_OC3M_1|TIM_CCMR2_OC3M_2 ) | \
                    TIM_CCMR2_OC4PE | (TIM_CCMR2_OC4M_1|TIM_CCMR2_OC4M_2 );

  if (timWGM_OCMASK & 1) timInst->CCER |= TIM_CCER_CC1E ;
  if (timWGM_OCMASK & 2) timInst->CCER |= TIM_CCER_CC2E ;
  if (timWGM_OCMASK & 4) timInst->CCER |= TIM_CCER_CC3E ;
  if (timWGM_OCMASK & 8) timInst->CCER |= TIM_CCER_CC4E ;

  timInst->BDTR |= TIM_BDTR_MOE;

  if (((timWGM_OCMASK>>4) & 0xf) == 1) // single/dual slope
    timInst->CR1 |= TIM_CR1_CEN | TIM_CR1_ARPE | TIM_CR1_CMS_0;
  else
    timInst->CR1 |= TIM_CR1_CEN | TIM_CR1_ARPE;
}
//---------------------------------------------------------------------------
void timPwmSET(TIM_TypeDef* timInst,uint8_t l_chan,uint16_t l_pwm)
{
    if (l_chan == 1) timInst->CCR1 = l_pwm; else
    if (l_chan == 2) timInst->CCR2 = l_pwm; else
    if (l_chan == 3) timInst->CCR3 = l_pwm; else
    if (l_chan == 4) timInst->CCR4 = l_pwm; else
                           timInst->CCR1 = l_pwm;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

