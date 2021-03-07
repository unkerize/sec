//-----------------------------------------------------------------------------
// Project config
//-----------------------------------------------------------------------------
#ifndef _HK_CONFIG_H
#define _HK_CONFIG_H
//-----------------------------------------------------------------------------
// stacks: CSTACK, RETSTACK
#define _STACK_SIZE 0x1000
//-----------------------------------------------------------------------------
// Crystal freq, Hz
#define _CRYSTAL 8000000
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// clock distribution
//  osc ->[pll] ->AHB -> core, apb1, apb2
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// osc main source: 1=HSI, 2=HSE
//#define _OSC_SOURCE _OSC_HSI
#define _OSC_SOURCE _OSC_HSE

// ahb clk source: 1=HSI, 2=HSE, 3=PLL
#define _CLK_AHB_SOURCE _CLK_AHB_PLL

// if HSE->PLL prediv
#define _HSE_PLL_DIV 1  // (1,2,3..16)

// pll multiplier (2,3,4..16)
#define _PLL_MUL 9

// ahb, apb1, apb2, adc prediv
#define _CLK_AHB_DIV   1     // 1,2..512
#define _CLK_APB1_DIV  2     // 1,2,4,8,16
#define _CLK_APB2_DIV  1     // 1,2,4,8,16
#define _CLK_ADC_DIV   2     // 2,4,6,8

#define _CLK_AHB2_DIV  1     //
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// system manager options
#define tim1kHzCallSize 10      // tim1kHzNum
#define stCallSize      10      // stNum
#define rtsCallSize     2       // rtsNum

// overflow buffer TRAP
#define stOvfTrap
#define rtsOvfTrap
//-----------------------------------------------------------------------------
// CRC config (use math or table)
// not def or = 0 then not used
#define _CRC8_TAB
#define _CRC16_TAB
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
#include "brd_discovery_f3.h"
//#include "brd_discovery_vl.h"
//-----------------------------------------------------------------------------
// debug/release flag
#define _DEBUG    0
//-----------------------------------------------------------------------------
#endif //_HK_CONFIG_H
//-----------------------------------------------------------------------------
