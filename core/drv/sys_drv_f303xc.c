//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
// clkInit
//-------------------------------------------------------------------------
void clkInit(void)
{
#if _CLK_WAIT_STATE != 0
  FLASH->ACR = _CLK_WAIT_STATE | FLASH_ACR_PRFTBE | FLASH_ACR_HLFCYA*0;
#endif

//  FLASH->ACR = FLASH_ACR_PRFTBE | (uint32_t)FLASH_ACR_LATENCY_1;

#ifdef _CLK_HSE_USE
  RCC->CR |= RCC_CR_HSEON;

  while ((RCC->CR & RCC_CR_HSERDY) != RCC_CR_HSERDY) {}; // wait for stable
#endif

#ifdef _CLK_PLL_USE

  #ifdef _CLK_HSE_USE
    RCC->CFGR2 = (uint32_t) _HSE_PLL_DIV_MASK;
  #endif

  RCC->CFGR |= (uint32_t) _PLL_MUL_MASK | _PLL_OSC_MASK;

  RCC->CR |= RCC_CR_PLLON;
    while ((RCC->CR & RCC_CR_PLLRDY) != RCC_CR_PLLRDY) {};  // wait for stable
#endif

  RCC->CFGR |= (uint32_t) _CLK_AHB_DIV_MASK | _CLK_APB1_DIV_MASK | \
                          _CLK_APB2_DIV_MASK ;

  RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
  RCC->CFGR |= _CLK_AHB_SOURCE_MASK;

  RCC->CFGR2 |= (uint32_t) _CLK_ADC_DIV_MASK | (_CLK_ADC_DIV_MASK/RCC_CFGR2_ADCPRE12_0*RCC_CFGR2_ADCPRE34_0);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
