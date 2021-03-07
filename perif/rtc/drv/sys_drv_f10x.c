//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
uint8_t rtcSecEventID = 0xff;
#define rtcPS (rtc_CRYSTAL/1)
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
  volatile uint32_t* reg;
  uint8_t pos;

  uint8_t dir;
  uint8_t outtype;
  uint8_t intype;
  uint8_t outspeed;
  uint16_t alter;

  if (pin<8) reg = &port->CRL ;else reg = &port->CRH;
  pos = (pin % 8) * 4;

  dir      = pincfg  &  0x3;
  outtype  = pincfg  & (0x3 << 2);
  intype   = pincfg  & (0x3 << 4);
  outspeed = pincfg  & (0x3 << 6);

  *reg &= ~(0xf << pos);

  if(dir == gpd_input)
  {
      *reg |= gpm_input << pos;

      if (intype == gpit_pullup)
      {
          *reg |= gpf_input_pp << (pos+2);
          port->ODR |= 1<<pin;
      }

      if (intype == gpit_pulldn)
      {
          *reg |= gpf_input_pp << (pos+2);
          port->ODR &= ~(1<<pin);
      }

      if (intype == gpit_nopull)
      {
          *reg |=  gpf_input << (pos+2);
      }

      return;
  }

  if(dir == gpd_analog)
  {
      *reg |= (gpm_input << pos) | (gpf_analog << (pos+2));
      return;
  }

  alter    = pincfg  & (0x3 << 8);

  if(dir == gpd_output)
  {
      switch (outspeed)
      {
      case  gpos_lo:
          *reg |= gpm_out2 << pos;
          break;
      case  gpos_me:
          *reg |= gpm_out10 << pos;
          break;
      case  gpos_hi:
          *reg |= gpm_out50 << pos;
          break;
      default:
          *reg |= gpm_out50 << pos;
          break;
      }

      if (alter == gpaf_no)
      {
          if(outtype == gpot_pushpull)  *reg |=  gpf_outgen_pp << (pos+2);
          else                          *reg |=  gpf_outgen_od << (pos+2); //gpot_opendrain
      } else // alter == gpaf_alter
      {
          if(outtype == gpot_pushpull) *reg |=  gpf_outalt_pp << (pos+2);
          else                         *reg |=  gpf_outalt_od << (pos+2);  //gpot_opendrain
      }
  }
}
//---------------------------------------------------------------------------
// rtc DRIVER
//---------------------------------------------------------------------------
void RTC_IRQHandler(void)
{
    if (rtcSecEventID != 0xff) stSetEvent(rtcSecEventID);

    RTC->CRL &= ~RTC_CRL_SECF;
}
//---------------------------------------------------------------------------
void rtcInit(fptr lpSecEvent)
{
    RCC->APB1ENR |= RCC_APB1ENR_PWREN | RCC_APB1ENR_BKPEN;
    PWR->CR |= PWR_CR_DBP;

    if ( !((RCC->BDCR & RCC_BDCR_LSEON) && (RCC->BDCR & RCC_BDCR_LSERDY) &&\
        ((RCC->BDCR & RCC_BDCR_RTCSEL) == RCC_BDCR_RTCSEL_LSE) && (RCC->BDCR & RCC_BDCR_RTCEN)))
    {
        RCC->BDCR |= RCC_BDCR_LSEON;

        while (! (RCC->BDCR & RCC_BDCR_LSERDY)) {};

        RCC->BDCR &= ~RCC_BDCR_RTCSEL;
        RCC->BDCR |= RCC_BDCR_RTCSEL_LSE;
        RCC->BDCR |= RCC_BDCR_RTCEN;
    }

    while (!(RTC->CRL & RTC_CRL_RTOFF)) {};

    if ( !((RTC->PRLL == (rtcPS & 0xffff)) && (RTC->PRLH == ( rtcPS>>16)) ) )
    {
        RTC->CRL |= RTC_CRL_CNF;

        RTC->PRLL = rtcPS & 0xffff;
        RTC->PRLH = (rtcPS>>16) & 0xffff;

        RTC->CRL &= ~RTC_CRL_CNF;

        while (!(RTC->CRL & RTC_CRL_RTOFF)) {};
    }

    if (lpSecEvent != 0)
    {
        RTC->CRL |= RTC_CRL_CNF;

        RTC->CRH = RTC_CRH_SECIE;

        RTC->CRL &= ~RTC_CRL_CNF;

        while (!(RTC->CRL & RTC_CRL_RTOFF)) {};

        rtcSecEventID = stAddTask(lpSecEvent, stf_event);

        NVIC->ISER[RTC_IRQn/32] = 1<<(RTC_IRQn%32)  ;
    } else
    {
        RTC->CRL |= RTC_CRL_CNF;

        RTC->CRH = 0;

        RTC->CRL &= ~RTC_CRL_CNF;

        while (!(RTC->CRL & RTC_CRL_RTOFF)) {};
    }
}
//---------------------------------------------------------------------------
void rtcWrite(uint32_t systime)
{
    while (!(RTC->CRL & RTC_CRL_RTOFF)) {};

    RTC->CRL |= RTC_CRL_CNF;

    RTC->CNTH  = systime >> 16;
    RTC->CNTL  = systime &0xffff;

    RTC->CRL &= ~RTC_CRL_CNF;

    while (!(RTC->CRL & RTC_CRL_RTOFF)) {};
}
//---------------------------------------------------------------------------
uint32_t rtcRead(void)
{
    uint16_t stl;
    uint16_t sth;

    while (!(RTC->CRL & RTC_CRL_RTOFF)) {};

    do
    {
        sth = RTC->CNTH;
        stl = RTC->CNTL;
    } while(RTC->CNTH != sth);

    return (sth << 16) | stl;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
// timers
//---------------------------------------------------------------------------
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
  if (timInst == TIM4) RCC->APB1ENR |= RCC_APB1ENR_TIM4EN; else
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
void gpioEN_RCC(uint32_t rccMask)
{
  if (rccMask & gpioRCC_A) RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
  if (rccMask & gpioRCC_B) RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
  if (rccMask & gpioRCC_C) RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
  if (rccMask & gpioRCC_D) RCC->APB2ENR |= RCC_APB2ENR_IOPDEN;
  if (rccMask & gpioRCC_E) RCC->APB2ENR |= RCC_APB2ENR_IOPEEN;
	
#if defined(RCC_APB2ENR_IOPFEN)	
  if (rccMask & gpioRCC_F) RCC->APB2ENR |= RCC_APB2ENR_IOPFEN;
#endif	
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
