//-----------------------------------------------------------------------------
// spi main header module (including)
//-----------------------------------------------------------------------------
#ifndef _HK_SPI_H
#define _HK_SPI_H

#include "system.h"
#include "spicfg.h"
#include "spi_types.h"
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// spi OPTIONS  bits:
// 0..1   spiMODE 0..3
// 2..4   spiBR divider 0..7 (2..256)
// 5      spiMSB/LSB first 0/1
// 6..7   spiOScom style (0..2)
// 8..15  os handle (byte)

#define spioMODE_bp    (0)
#define spioMODE_msk   (0x03)

#define spioBR_bp      (2)
#define spioBR_msk     (0x07)

#define spioMSBF_bp    (5)
#define spioMSBF_msk   (0x01)

#define spioOScom_bp   (6)
#define spioOScom_msk  (0x03)

#define spioOSindx_bp  (8)
#define spioOSindx_msk (0xff)
//----------------------------------------------------------------------
//----------------------------------------------------------------------
#define spioOsNON  (0<<spioOScom_bp)
#define spioOsST   (1<<spioOScom_bp)
#define spioOsRTS  (2<<spioOScom_bp)
//----------------------------------------------------------------------
//----------------------------------------------------------------------
// spi mode masks
#define spioMODE0 (0<<spioMODE_bp)
#define spioMODE1 (1<<spioMODE_bp)
#define spioMODE2 (2<<spioMODE_bp)
#define spioMODE3 (3<<spioMODE_bp)

// spi BaudeRate div
#define spioBR2    (0x0<<spioBR_bp)
#define spioBR4    (0x1<<spioBR_bp)
#define spioBR8    (0x2<<spioBR_bp)
#define spioBR16   (0x3<<spioBR_bp)
#define spioBR32   (0x4<<spioBR_bp)
#define spioBR64   (0x5<<spioBR_bp)
#define spioBR128  (0x6<<spioBR_bp)
#define spioBR256  (0x7<<spioBR_bp)

// spi MSB/LSB first
#define spioMSBF (0<<spioMSBF_bp)
#define spioLSBF (1<<spioMSBF_bp)
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
#if defined(STM32F030) || defined(STM32F031) || \
    defined(STM32F051) || defined(STM32F072) || defined(STM32F042)
  #include "drv\spi_drv_f0xx.h"
#endif

#if defined(STM32F10X_LD) || defined(STM32F10X_LD_VL) || \
    defined(STM32F10X_MD) || defined(STM32F10X_MD_VL) || \
    defined(STM32F10X_HD) || defined(STM32F10X_HD_VL) || \
    defined(STM32F10X_XL) || defined (STM32F10X_CL)
  #include "drv\spi_drv_f10x.h"
#endif

#if defined(STM32F303x6) || defined(STM32F303x8) || \
    defined(STM32F303xB) || defined(STM32F303xC) || \
    defined(STM32F334)
  #include "drv\spi_drv_f303xc.h"
#endif
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void spiTxBlock(uint8_t lSpi, uint8_t* ltxBuf, uint8_t* lrxBuf, uint16_t lSize);
uint8_t spiBusy(uint8_t lSpi);
uint8_t spiAdd(SPI_TypeDef* lspi_port, uint32_t lspi_options);
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void spiSetDummy(uint8_t spih, uint8_t lDummy);
uint8_t spiTxByte(uint8_t spih, uint8_t txValue);
uint8_t spiTxByteAsync(uint8_t spih, uint8_t txValue);
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
#endif //module exit
