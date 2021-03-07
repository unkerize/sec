
#include "system.h"
#include "sys_drv.h"

extern uint8_t sync_flag;
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
#if defined(STM32F030) || defined(STM32F031) || \
    defined(STM32F051) || defined(STM32F072) || defined(STM32F042)
  #include "drv\sys_drv_f0xx.c"
#endif

#if defined(STM32F10X_LD) || defined(STM32F10X_LD_VL) || \
    defined(STM32F10X_MD) || defined(STM32F10X_MD_VL) || \
    defined(STM32F10X_HD) || defined(STM32F10X_HD_VL) || \
    defined(STM32F10X_XL) || defined (STM32F10X_CL)
  #include "drv\sys_drv_f10x.c"
#endif

#if defined(STM32F303x6) || defined(STM32F303x8)
  #include "drv\sys_drv_f303x8.c"
#endif

#if defined(STM32F303xB) || defined(STM32F303xC)
  #include "drv\sys_drv_f303xc.c"
#endif

#if defined(STM32F334)
  #include "drv\sys_drv_f334.c"
#endif

#if defined(STM32G474)
  #include "drv\sys_drv_G474.c"
#endif
//-------------------------------------------------------------------------
// nvicInit
//-----------------------------------------------------------------------------
void nvicInit(void)
{
}
//-------------------------------------------------------------------------
// sync interrupt handler -> 1kHz
//-----------------------------------------------------------------------------
void SysTick_Handler(void)
{
    sync_flag = 1;      // up sync flag
};
//-----------------------------------------------------------------------------
// sync startup initialization
//-----------------------------------------------------------------------------
void syncInit(void)
{
  SysTick->VAL = sync_OCR ;

  SysTick->LOAD = sync_OCR ;

  SysTick->CTRL = SysTick_CTRL_ENABLE_Msk  | SysTick_CTRL_TICKINT_Msk;
}
//-----------------------------------------------------------------------------
// chip clock system init, interrupt controller init
//-----------------------------------------------------------------------------
void usecDelay(uint32_t usDel)
{
  uint32_t nST, start, delta;

  if (usDel == 0) return;

  nST = usDel*SysTick->LOAD/1000;
  start = SysTick->VAL;
        delta = 0;

  while ( delta < nST )
  { delta = start - SysTick->VAL;}
}

void msecDelay(uint16_t msDel)
{
  uint16_t i;
  if (msDel == 0) return;

  for (i = 0; i < msDel-1; i++) {usecDelay(msDel*1000);}
}
//-----------------------------------------------------------------------------
