
#include "system.h"
#include "uart.h"

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
#if defined(STM32F030) || defined(STM32F031) || \
    defined(STM32F051) || defined(STM32F072) || defined(STM32F042)
  #include "drv\uart_drv_f0xx.c"
#endif

#if defined(STM32F10X_LD) || defined(STM32F10X_LD_VL) || \
    defined(STM32F10X_MD) || defined(STM32F10X_MD_VL) || \
    defined(STM32F10X_HD) || defined(STM32F10X_HD_VL) || \
    defined(STM32F10X_XL) || defined (STM32F10X_CL)
  #include "drv\uart_drv_f10x.c"
#endif

// STM32F303x8
// STM32F303xB, STM32F303xC
#if defined(STM32F303x6) || defined(STM32F303x8)  || \
    defined(STM32F303xB) ||  defined(STM32F303xC)
  #include "drv\uart_drv_f303xc.c"
#endif

#if defined(STM32F334)
  #include "drv\uart_drv_f334.c"
#endif

#if defined(STM32G474)
  #include "drv\uart_drv_g474.c"
#endif

//-------------------------------------------------------------------------
//-------------------------------------------------------------------------


