
#include "system.h"
#include "app.h"

int main( void )
{
    sysInit();
    
    appInit();

  while(1)
  {
    syncMain();

    stMain();

#if rtsCallSize != 0
//    rtsMain();
#endif
//    sysMain();
  }
}


void SystemInit(void)
{
  #if (__FPU_PRESENT == 1) && (__FPU_USED == 1)
    SCB->CPACR |= ((3UL << 10*2)|(3UL << 11*2));  /* set CP10 and CP11 Full Access */
  #endif
}
