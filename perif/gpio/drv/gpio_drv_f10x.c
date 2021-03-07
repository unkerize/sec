//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void gpioCFG(GPIO_TypeDef* port,uint8_t pin, uint32_t pincfg)
{
  volatile uint32_t* reg;
  uint8_t pos;

  uint8_t dir;
  uint8_t outtype;
  uint8_t intype;
  uint8_t outspeed;
  uint8_t alter;
//  uint8_t alter_n;

  dir      = pincfg & gpd_msk;
  outtype  = pincfg & gpot_msk;
  intype   = pincfg & gpit_msk;
  outspeed = pincfg & gpos_msk;
  alter    = pincfg & gpaf_msk;
//  alter_n  = pincfg >> gpaf_indx_bp)  & gpaf_indx_msk;

  if (pin<8) reg = &port->CRL ;else reg = &port->CRH;
  pos = (pin % 8) * 4;

  *reg &= ~(0xf << pos);

  if(dir == gpd_input)
  {
      if (intype == gpit_nopull)
      {
        *reg |=  0x00 << pos;
        *reg |=  0x01 << (pos+2);
      } else

      if (intype == gpit_pullup)
      {
        *reg |=  0x00 << pos;
        *reg |=  0x02 << (pos+2);
         port->ODR |= 1<<pin;
      } else

      if (intype == gpit_pulldn)
      {
        *reg |=  0x00 << pos;
        *reg |=  0x02 << (pos+2);
         port->ODR &= ~(1<<pin);
      }
      return;
  }

  if(dir == gpd_analog) return; // mode, cnf bits already=0

  if(dir == gpd_output)
  {
      if (outspeed == gpos_lo) *reg |= 0x02 << pos; else
      if (outspeed == gpos_me) *reg |= 0x01 << pos; else
      if (outspeed == gpos_hi) *reg |= 0x03 << pos;

      if (alter == gpaf_no)
      {
          if(outtype == gpot_pushpull)  *reg |=  0x00 << (pos+2);
          else                          *reg |=  0x01 << (pos+2); //opendrain
      } else // alter == gpaf_alter
      {
          if(outtype == gpot_pushpull) *reg |=  0x02 << (pos+2);
          else                         *reg |=  0x03 << (pos+2);  //opendrain
      }
  }
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void gpioEN_RCC(GPIO_TypeDef* port)
{
  if (port == GPIOA) RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;

#if defined(RCC_APB2ENR_IOPBEN)
  if (port == GPIOB) RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
#endif

#if defined(RCC_APB2ENR_IOPCEN)
  if (port == GPIOC) RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
#endif

#if defined(RCC_APB2ENR_IOPDEN)
  if (port == GPIOD) RCC->APB2ENR |= RCC_APB2ENR_IOPDEN;
#endif

#if defined(RCC_APB2ENR_IOPEEN)
  if (port == GPIOE) RCC->APB2ENR |= RCC_APB2ENR_IOPEEN;
#endif

#if defined(RCC_APB2ENR_IOPFEN)
  if (port == GPIOF) RCC->APB2ENR |= RCC_APB2ENR_IOPFEN;
#endif

#if defined(RCC_APB2ENR_IOPGEN)
  if (port == GPIOG) RCC->APB2ENR |= RCC_APB2ENR_IOPGEN;
#endif
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
