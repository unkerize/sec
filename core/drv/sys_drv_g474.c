//_HSE_PLL_DIV_MASK (_HSE_PLL_DIV-1)
//_PLL_MUL_MASK ( (_PLL_MUL)*RCC_PLLCFGR_PLLM_0)

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
// clkInit
//-------------------------------------------------------------------------
void clkInit(void)
{
  PWR->CR5 = 0; // core voltage range 1, boost mode (max power)

// set wait states, enables prefetch, data cache, code cache
  FLASH->ACR = (_CLK_WAIT_STATE) | FLASH_ACR_PRFTEN | FLASH_ACR_ICEN | FLASH_ACR_DCEN | FLASH_ACR_DBG_SWEN;


#ifdef _CLK_HSE_USE
  RCC->CR |= RCC_CR_HSEON;

  while ((RCC->CR & RCC_CR_HSERDY) != RCC_CR_HSERDY) {}; // wait for stable
#endif

#ifdef _CLK_PLL_USE

    RCC->PLLCFGR = (uint32_t) _HSE_PLL_DIV_MASK | _PLL_MUL_MASK | _PLL_OSC_MASK | RCC_PLLCFGR_PLLREN;
//  #ifdef _CLK_HSE_USE
//    RCC->CFGR2 = (uint32_t) _HSE_PLL_DIV_MASK;
//  #endif

//  RCC->CFGR |= (uint32_t) _PLL_MUL_MASK | _PLL_OSC_MASK;

  RCC->CR |= RCC_CR_PLLON;
    while ((RCC->CR & RCC_CR_PLLRDY) != RCC_CR_PLLRDY) {};  // wait for stable
#endif

  RCC->CFGR |= (uint32_t) _CLK_AHB_DIV_MASK | _CLK_APB1_DIV_MASK |
                          _CLK_APB2_DIV_MASK | _CLK_AHB_SOURCE_MASK;

//  RCC->CFGR2 |= (uint32_t) _CLK_ADC_DIV_MASK;
  RCC->CCIPR = RCC_CCIPR_ADC345SEL_0*2 | RCC_CCIPR_ADC12SEL_0*2;

}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
