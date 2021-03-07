//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
// clkInit
//-------------------------------------------------------------------------
void clkInit(void)
{

#if _CLK_WAIT_STATE != 0
  FLASH->ACR |= _CLK_WAIT_STATE;
#endif


#ifdef _CLK_HSE_USE
  RCC->CR |= RCC_CR_HSEON;

  while ((RCC->CR & RCC_CR_HSERDY) != RCC_CR_HSERDY) {}; // wait for stable
#endif

#ifdef _CLK_PLL_USE

  #ifdef _CLK_HSE_USE
    #ifdef STM32F10X_MD_VL
      RCC->CFGR2 = (uint32_t) _HSE_PLL_DIV_MASK;
    #endif

    RCC->CFGR |= (uint32_t) ((_HSE_PLL_DIV_MASK & 1) * RCC_CFGR_PLLXTPRE);
  #endif

  RCC->CFGR |= (uint32_t) _PLL_MUL_MASK | _PLL_OSC_MASK;

  RCC->CR |= RCC_CR_PLLON;
    while ((RCC->CR & RCC_CR_PLLRDY) != RCC_CR_PLLRDY) {};  // wait for stable
#endif

  RCC->CFGR |= (uint32_t) _CLK_AHB_DIV_MASK | _CLK_APB1_DIV_MASK | \
                          _CLK_APB2_DIV_MASK | _CLK_ADC_DIV_MASK;

  RCC->CFGR |= _CLK_AHB_SOURCE_MASK;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
