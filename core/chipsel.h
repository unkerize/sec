//---------------------------------------------------------------------------
#ifndef _HK_CHIPSEL_H
#define _HK_CHIPSEL_H


//---------------------------------------------------------------------------
#include "config.h"
//---------------------------------------------------------------------------

#if defined(STM32F030) || defined(STM32F031) || \
    defined(STM32F051) || defined(STM32F072) || defined(STM32F042)

  #include <stm32f0xx.h>
#endif

#if defined (STM32F101x6) || defined (STM32F102x6) || defined(STM32F103x6) || \
    defined (STM32F100x6) || \
    defined (STM32F101xB) || defined (STM32F102xB) || defined(STM32F103xB) || \
    defined (STM32F100xB) || \
    defined (STM32F101xE) || defined (STM32F102xE) || defined(STM32F103xE) || \
    defined (STM32F100xE) || \
    defined (STM32F101xG) || defined (STM32F102xG) || defined(STM32F103xG) || \
    defined (STM32F100xG)

#include "drv\chips_10x.h"
//    defined(STM32F10X_LD) || defined(STM32F10X_LD_VL) || \
//    defined(STM32F10X_MD) || defined(STM32F10X_MD_VL) || \
//    defined(STM32F10X_HD) || defined(STM32F10X_HD_VL) || \
//    defined(STM32F10X_XL) || defined(STM32F10X_CL)
  #include <stm32f10x.h>
#endif


#if defined(STM32F303x6) || defined(STM32F303x8)
  #include <stm32f303x8.h>
#endif


#if defined(STM32F303xB) || defined(STM32F303xC)
  #include <stm32f303xc.h>
#endif

#if defined(STM32F334)
  #include <stm32f334x8.h>
#endif

#if defined(STM32G474)
  #include <stm32g474xx.h>
#endif

//---------------------------------------------------------------------------
#endif //_HK_CHIPSEL_H
