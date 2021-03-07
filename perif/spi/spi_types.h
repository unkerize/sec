//-----------------------------------------------------------------------------
#ifndef _HK_SPICONST_H
#define _HK_SPICONST_H
//-----------------------------------------------------------------------------
#define spiOsNON  (0)
#define spiOsST   (1)
#define spiOsRTS  (2)
//-----------------------------------------------------------------------------
typedef struct {
  uint8_t  osComOpt;
  uint8_t  osComIndex;
  uint8_t  busy;

  SPI_TypeDef* port;
  DMA_Channel_TypeDef* txDma;
  DMA_Channel_TypeDef* rxDma;

  uint8_t  wDummy;
  uint8_t  rDummy;
} spiType;
//-----------------------------------------------------------------------------
// defines chip-specific const
#if defined(STM32F030) || defined(STM32F031) || \
    defined(STM32F051) || defined(STM32F072) || defined(STM32F042)
#endif

#if defined(STM32F10X_LD) || defined(STM32F10X_LD_VL) || \
    defined(STM32F10X_MD) || defined(STM32F10X_MD_VL) || \
    defined(STM32F10X_HD) || defined(STM32F10X_HD_VL) || \
    defined(STM32F10X_XL) || defined (STM32F10X_CL)
#endif

#if defined(STM32F303xB) || defined(STM32F303xC)
#endif

#endif //module exit
