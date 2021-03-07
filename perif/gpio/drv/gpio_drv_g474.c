//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void gpioCFG(GPIO_TypeDef* port,uint8_t pin, uint32_t pincfg)
{
  uint8_t dir;
  uint8_t outtype;
  uint8_t intype;
  uint8_t outspeed;
  uint16_t alter;
  uint8_t alterF;

  dir      = pincfg & gpd_msk;
  outtype  = pincfg & gpot_msk;
  intype   = pincfg & gpit_msk;
  outspeed = pincfg & gpos_msk;
  alter    = pincfg & gpaf_msk;
  alterF = pincfg >> gpaf_indx_bp;

  if(dir == gpd_analog)
  {
      port->MODER |= 0x3 << (pin*2);
      return;
  }

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


  if (alter == gpaf_alter)
  {
      port->MODER |= 0x2 << (pin*2);

      port->AFR[pin/8] &= ~(0xf<<((pin%8) *4));
      port->AFR[pin/8] |= alterF<<((pin%8)*4);
      return;
  }


  if(dir == gpd_input) return;

  if(dir == gpd_output) {port->MODER |= 0x1 << (pin*2);}
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void gpioEN_RCC(GPIO_TypeDef* port)
{
  if (port == GPIOA) RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;

#ifdef RCC_AHB2ENR_GPIOBEN
  if (port == GPIOB) RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
#endif

#ifdef RCC_AHB2ENR_GPIOCEN
  if (port == GPIOC) RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;
#endif

#ifdef RCC_AHB2ENR_GPIODEN
  if (port == GPIOD) RCC->AHB2ENR |= RCC_AHB2ENR_GPIODEN;
#endif

#ifdef RCC_AHB2ENR_GPIOEEN
  if (port == GPIOE) RCC->AHB2ENR |= RCC_AHB2ENR_GPIOEEN;
#endif

#ifdef RCC_AHB2ENR_GPIOFEN
  if (port == GPIOF) RCC->AHB2ENR |= RCC_AHB2ENR_GPIOFEN;
#endif

#ifdef RCC_AHB2ENR_GPIOGEN
  if (port == GPIOG) RCC->AHB2ENR |= RCC_AHB2ENR_GPIOGEN;
#endif

#ifdef RCC_AHB2ENR_GPIOHEN
  if (port == GPIOH) RCC->AHB2ENR |= RCC_AHB2ENR_GPIOHEN;
#endif
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

