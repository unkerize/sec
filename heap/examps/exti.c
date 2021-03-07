
/*		
		SYSCFG->EXTICR[3] = 0;
		EXTI->FTSR = (1 << 13);
		EXTI->IMR =  (1 << 13);
    NVIC_EnableIRQ(EXTI4_15_IRQn);
*/		

void EXTI4_15_IRQHandler (void)
{
  pin_set(GPIOC, 9);
	
	EXTI->PR =  (1 << 13);
	
  pin_set(GPIOC, 9);
  pin_set(GPIOC, 9);
  pin_set(GPIOC, 9);
  pin_set(GPIOC, 9);
  pin_set(GPIOC, 9);
  pin_clr(GPIOC, 9);
}
