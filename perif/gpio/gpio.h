//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
#include "system.h"
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
#ifndef _HK_GPIO_H
#define _HK_GPIO_H
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
// gpio options, bits
//  0..1 direction: 0=input, 1=output, 2=analog
//  2    output:    0=pushpull, 1= open-drain (npn)
//  3..4 input:     0=no pull, 1=pull up, 2=pull down
//  5..6 output speed: 0=low, 1=med, 2=hi
//  7    alternative:  0=non, 1=alter f
//  8..11 alter f number (cortex m0, m4)

#define gpd_bp        0
#define gpot_bp       2
#define gpit_bp       3
#define gpos_bp       5
#define gpaf_bp       7
#define gpaf_indx_bp  8

#define gpd_msk     (0x03<<gpd_bp)
#define gpot_msk    (0x01<<gpot_bp)
#define gpit_msk    (0x03<<gpit_bp)
#define gpos_msk    (0x03<<gpos_bp)
#define gpaf_msk    (0x01<<gpaf_bp)

#define gpaf_indx_msk  0x0f

// GPIO direction
#define gpd_input      (0<<gpd_bp)
#define gpd_output     (1<<gpd_bp)
#define gpd_analog     (2<<gpd_bp)

// GPIO output type
#define gpot_pushpull      (0<<gpot_bp)
#define gpot_opendrain     (1<<gpot_bp)

// GPIO input type
#define gpit_nopull     (0<<gpit_bp)
#define gpit_pullup     (1<<gpit_bp)
#define gpit_pulldn     (2<<gpit_bp)

// GPIO output speed
#define gpos_lo     (0<<gpos_bp)
#define gpos_me     (1<<gpos_bp)
#define gpos_hi     (2<<gpos_bp)

#define gpaf_no        (0<<gpaf_bp)
#define gpaf_alter     (1<<gpaf_bp)
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
#define gpioNON 0x00
#define gpioPA  0x01
#define gpioPB  0x02
#define gpioPC  0x03
#define gpioPD  0x04
#define gpioPE  0x05
#define gpioPF  0x06
#define gpioPG  0x07

#define gpioPH  0x08
#define gpioPI  0x09
#define gpioPJ  0x0A
#define gpioPK  0x0B
//-------------------------------------------------------------------------
void gpioCFG(GPIO_TypeDef* port,uint8_t pin, uint32_t pincfg);
//-------------------------------------------------------------------------
// lgpioP = port gpioPA..gpioPF
void gpioEN_RCC(GPIO_TypeDef* port);
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//GPIO_TypeDef* gpioGetPort(uint8_t lgpioP);
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------

#endif //_HK_GPIO_H
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
