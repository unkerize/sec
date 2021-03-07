// STM32F10X_LD    : STM32F101xx, STM32F102xx and STM32F103xx, 16-32 k.
// STM32F10X_LD_VL : STM32F100xx 16-32 Kbytes.
// STM32F10X_MD    : STM32F101xx, STM32F102xx and STM32F103xx. 64-128 k
// STM32F10X_MD_VL : STM32F100xx, 64-128 k
// STM32F10X_HD    : STM32F101xx, STM32F103xx, 256-512 k
// STM32F10X_HD_VL : STM32F100xx, 256-512 k
// STM32F10X_XL    : STM32F101xx, STM32F103xx, 512-1024 k
// STM32F10X_CL    : STM32F105xx, STM32F107xx

//-PERIF LIST---------------------------------------------
#if defined (STM32F10X_LD)

#define PERIF_TIM1
#define PERIF_TIM2
#define PERIF_TIM3

#define TIM_COUNT 3

#define PERIF_USART1
#define PERIF_USART2

#define PERIF_SPI1

#define PERIF_I2C1

#define PERIF_ADC12

//-PERIF LIST---------------------------------------------
#elif defined (STM32F10X_LD_VL)

#define PERIF_TIM1
#define PERIF_TIM2
#define PERIF_TIM3
#define PERIF_TIM6
#define PERIF_TIM7
#define PERIF_TIM15
#define PERIF_TIM16
#define PERIF_TIM17

#define TIM_COUNT 17

#define PERIF_USART1
#define PERIF_USART2

#define PERIF_SPI1

#define PERIF_I2C1

#define PERIF_ADC

//-PERIF LIST---------------------------------------------
#elif defined (STM32F10X_MD)

#define PERIF_TIM1
#define PERIF_TIM2
#define PERIF_TIM3
#define PERIF_TIM4

#define TIM_COUNT 4

#define PERIF_USART1
#define PERIF_USART2
#define PERIF_USART3

#define PERIF_SPI1
#define PERIF_SPI2

#define PERIF_I2C1
#define PERIF_I2C2

#define PERIF_ADC12

//-PERIF LIST---------------------------------------------
#elif defined (STM32F10X_MD_VL)

#define PERIF_TIM1
#define PERIF_TIM2
#define PERIF_TIM3
#define PERIF_TIM4
#define PERIF_TIM6
#define PERIF_TIM7
#define PERIF_TIM15
#define PERIF_TIM16
#define PERIF_TIM17

#define TIM_COUNT 17

#define PERIF_USART1
#define PERIF_USART2
#define PERIF_USART3

#define PERIF_SPI1
#define PERIF_SPI2

#define PERIF_I2C1
#define PERIF_I2C2

#define PERIF_ADC
//-PERIF LIST---------------------------------------------
#elif defined (STM32F10X_HD)

#define PERIF_TIM1
#define PERIF_TIM2
#define PERIF_TIM3
#define PERIF_TIM4
#define PERIF_TIM5
#define PERIF_TIM6
#define PERIF_TIM7
#define PERIF_TIM8

#define TIM_COUNT 8

#define PERIF_USART1
#define PERIF_USART2
#define PERIF_USART3
#define PERIF_USART4
#define PERIF_USART5

#define PERIF_SPI1
#define PERIF_SPI2
#define PERIF_SPI3

#define PERIF_I2C1
#define PERIF_I2C2

#define PERIF_ADC12
#define PERIF_ADC3
//-PERIF LIST---------------------------------------------
#elif defined (STM32F10X_HD_VL)

#define PERIF_TIM1
#define PERIF_TIM2
#define PERIF_TIM3
#define PERIF_TIM4
#define PERIF_TIM5
#define PERIF_TIM6
#define PERIF_TIM7
#define PERIF_TIM12
#define PERIF_TIM13
#define PERIF_TIM14
#define PERIF_TIM15
#define PERIF_TIM16
#define PERIF_TIM17

#define TIM_COUNT 17

#define PERIF_USART1
#define PERIF_USART2
#define PERIF_USART3
#define PERIF_USART4
#define PERIF_USART5

#define PERIF_SPI1
#define PERIF_SPI2
#define PERIF_SPI3

#define PERIF_I2C1
#define PERIF_I2C2

#define PERIF_ADC1

//-PERIF LIST---------------------------------------------
#elif defined (STM32F10X_XL)

#define PERIF_TIM1
#define PERIF_TIM2
#define PERIF_TIM3
#define PERIF_TIM4
#define PERIF_TIM5
#define PERIF_TIM6
#define PERIF_TIM7
#define PERIF_TIM8
#define PERIF_TIM9
#define PERIF_TIM10
#define PERIF_TIM11
#define PERIF_TIM12
#define PERIF_TIM13
#define PERIF_TIM14

#define TIM_COUNT 14

#define PERIF_USART1
#define PERIF_USART2
#define PERIF_USART3
#define PERIF_USART4
#define PERIF_USART5

#define PERIF_SPI1
#define PERIF_SPI2
#define PERIF_SPI3

#define PERIF_I2C1
#define PERIF_I2C2

#define PERIF_ADC12
#define PERIF_ADC3

//-PERIF LIST---------------------------------------------
#elif defined (STM32F10X_CL)

#define PERIF_TIM1
#define PERIF_TIM2
#define PERIF_TIM3
#define PERIF_TIM4
#define PERIF_TIM5
#define PERIF_TIM6
#define PERIF_TIM7

#define TIM_COUNT 7

#define PERIF_USART1
#define PERIF_USART2
#define PERIF_USART3
#define PERIF_USART4
#define PERIF_USART5

#define PERIF_SPI1
#define PERIF_SPI2
#define PERIF_SPI3

#define PERIF_I2C1
#define PERIF_I2C2

#define PERIF_ADC12

#endif

#define TIM_COUNT_MAX 17

