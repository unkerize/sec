//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void dac_init(void)
{
#ifdef RCC_APB1ENR_DACEN	
  RCC->APB1ENR |= RCC_APB1ENR_DACEN;

	DAC->CR = DAC_CR_EN1 | DAC_CR_EN2;

  DAC->DHR12R1 = 100;
  DAC->DHR12R2 = 100;
#endif
	
	
#ifdef RCC_APB1ENR_DAC1EN	
  RCC->APB1ENR |= RCC_APB1ENR_DAC1EN;

	DAC1->CR = DAC_CR_EN1 | DAC_CR_EN2;

  DAC1->DHR12R1 = 100;
  DAC1->DHR12R2 = 100;
#endif
}

