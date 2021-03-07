//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
#ifndef _HK_sys_stm32f100RB_H
#define _HK_sys_stm32f100RB_H
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
// F_CPU
#if   _CLK_SOURCE == _CLK_HSI
    #define _CLK_SPEED 8000000
#elif _CLK_SOURCE == _CLK_HSE
    #define _CLK_SPEED _CRYSTAL
#else
    #error "_CLK_SOURCE undefined"
#endif
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
#if defined(_PLL_MUL) && (_PLL_MUL > 1)

    #if (_PLL_MUL < 2) || (_PLL_MUL > 16)
        #error "_PLL_MUL incorrect"
    #endif

    #if (_PLL_DIV < 1) || (_PLL_DIV > 16)
        #error "_PLL_DIV incorrect"
    #endif

    #if   _CLK_SOURCE == _CLK_HSI
       #define F_CPU (_CLK_SPEED*_PLL_MUL/2)
    #elif _CLK_SOURCE == _CLK_HSE
       #define F_CPU (_CLK_SPEED*_PLL_MUL/_PLL_DIV)
    #endif
#else
    #define F_CPU (_CLK_SPEED)
#endif
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
#if   _CLK_APB2_DIV == 1
    #define _CLK_APB2_MASK 0x0
#elif _CLK_APB2_DIV == 2
    #define _CLK_APB2_MASK 0x4
#elif _CLK_APB2_DIV == 4
    #define _CLK_APB2_MASK 0x5
#elif _CLK_APB2_DIV == 8
    #define _CLK_APB2_MASK 0x6
#elif _CLK_APB2_DIV == 16
    #define _CLK_APB2_MASK 0x7
#else
    #error "_CLK_APB2_DIV incorrect"
#endif
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
#if   _CLK_APB1_DIV == 1
    #define _CLK_APB1_MASK 0x0
#elif _CLK_APB1_DIV == 2
    #define _CLK_APB1_MASK 0x4
#elif _CLK_APB1_DIV == 4
    #define _CLK_APB1_MASK 0x5
#elif _CLK_APB1_DIV == 8
    #define _CLK_APB1_MASK 0x6
#elif _CLK_APB1_DIV == 16
    #define _CLK_APB1_MASK 0x7
#else
    #error "_CLK_APB1_DIV incorrect"
#endif
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
#if   _CLK_AHB_DIV == 1
        #define _CLK_AHB_MASK 0x0
#elif _CLK_AHB_DIV == 2
        #define _CLK_AHB_MASK 0x8
#elif _CLK_AHB_DIV == 4
        #define _CLK_AHB_MASK 0x9
#elif _CLK_AHB_DIV == 8
        #define _CLK_AHB_MASK 0xA
#elif _CLK_AHB_DIV == 16
        #define _CLK_AHB_MASK 0xB
#elif _CLK_AHB_DIV == 64
        #define _CLK_AHB_MASK 0xC
#elif _CLK_AHB_DIV == 128
        #define _CLK_AHB_MASK 0xD
#elif _CLK_AHB_DIV == 256
        #define _CLK_AHB_MASK 0xE
#elif _CLK_AHB_DIV == 512
        #define _CLK_AHB_MASK 0xF
#else
    #error "_CLK_AHB_DIV incorrect"
#endif
//-------------------------------------------------------------------------
#define gpm_input     0
#define gpm_out10     1
#define gpm_out2      2
#define gpm_out50     3

#define gpf_analog     0
#define gpf_input      1
#define gpf_input_pp   2

#define gpf_outgen_pp  0
#define gpf_outgen_od  1
#define gpf_outalt_pp  2
#define gpf_outalt_od  3
//-------------------------------------------------------------------------
#endif //_HK_sys_stm32f100RB_H
//---------------------------------------------------------------------------
