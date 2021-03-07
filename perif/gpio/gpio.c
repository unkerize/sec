
#include "system.h"
#include "gpio.h"

#if defined(STM32F030) || defined(STM32F031) || \
    defined(STM32F051) || defined(STM32F072) || defined(STM32F042)
  #include "drv\gpio_drv_f0xx.c"
#endif

#if defined(STM32F10X_LD) || defined(STM32F10X_LD_VL) || \
    defined(STM32F10X_MD) || defined(STM32F10X_MD_VL) || \
    defined(STM32F10X_HD) || defined(STM32F10X_HD_VL) || \
    defined(STM32F10X_XL) || defined (STM32F10X_CL)
  #include "drv\gpio_drv_f10x.c"
#endif

#if defined(STM32F303xB) || defined(STM32F303xC)
  #include "drv\gpio_drv_f303xc.c"
#endif

#if defined(STM32F334)
  #include "drv\gpio_drv_f334.c"
#endif

#if defined(STM32G474)
  #include "drv\gpio_drv_g474.c"
#endif


/*
GPIO_TypeDef* gpioGetPort(uint8_t lgpioP)
{
  switch (lgpioP)
  {
    case gpioPA: return GPIOA;
    case gpioPB: return GPIOB;
    case gpioPC: return GPIOC;
    case gpioPD: return GPIOD;
    case gpioPE: return GPIOE;
#if defined(RCC_APB2ENR_IOPFEN)
    case gpioPF: return GPIOF;
#endif

#if defined(RCC_APB2ENR_IOPGEN)
    case gpioPG: return GPIOG;
#endif
    default: return 0;
  }
}
*/



