//-----------------------------------------------------------------------------
// spi main module (compiling)
#include "spi.h"
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
#if defined(STM32F030) || defined(STM32F031) || \
    defined(STM32F051) || defined(STM32F072) || defined(STM32F042)
  #include "drv\spi_drv_f0xx.c"
#endif

#if defined(STM32F10X_LD) || defined(STM32F10X_LD_VL) || \
    defined(STM32F10X_MD) || defined(STM32F10X_MD_VL) || \
    defined(STM32F10X_HD) || defined(STM32F10X_HD_VL) || \
    defined(STM32F10X_XL) || defined (STM32F10X_CL)
  #include "drv\spi_drv_f10x.c"
#endif

#if defined(STM32F303xC)
  #include "drv\spi_drv_f303xc.c"
#endif
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------