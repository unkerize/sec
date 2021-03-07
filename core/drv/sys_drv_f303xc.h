//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
#ifndef _HK_SYS_DRV_303XC_H
#define _HK_SYS_DRV_303XC_H
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
#define _INT_RC 8000000 // HSI = 8MHz
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
// test OSC/CLK params, determine maks

//------------------------------------------------------------
// _HSE_PLL_DIV (PREDIV1)
#ifndef _HSE_PLL_DIV
  #error "_HSE_PLL_DIV undefined"
#endif

#if (_HSE_PLL_DIV < 1) || (_HSE_PLL_DIV > 16)
  #error "_HSE_PLL_DIV out of range"
#endif

#define _HSE_PLL_DIV_MASK (_HSE_PLL_DIV-1)
//------------------------------------------------------------
// _PLL_MUL (PLLMUL)
#ifndef _PLL_MUL
  #error "_PLL_MUL undefined"
#endif

#if (_PLL_MUL < 2) || (_PLL_MUL > 16)
  #error "_PLL_MUL out of range"
#endif

#define _PLL_MUL_MASK ( (_PLL_MUL-2)*RCC_CFGR_PLLMUL_0 )
//------------------------------------------------------------
// _CLK_AHB_DIV
#ifndef _CLK_AHB_DIV
  #error "_CLK_AHB_DIV undefined"
#endif

#if   _CLK_AHB_DIV == 1
  #define _CLK_AHB_DIV_MASK RCC_CFGR_HPRE_DIV1
#elif _CLK_AHB_DIV == 2
  #define _CLK_AHB_DIV_MASK RCC_CFGR_HPRE_DIV2
#elif _CLK_AHB_DIV == 4
  #define _CLK_AHB_DIV_MASK RCC_CFGR_HPRE_DIV4
#elif _CLK_AHB_DIV == 8
  #define _CLK_AHB_DIV_MASK RCC_CFGR_HPRE_DIV8
#elif _CLK_AHB_DIV == 16
  #define _CLK_AHB_DIV_MASK RCC_CFGR_HPRE_DIV16
#elif _CLK_AHB_DIV == 64
  #define _CLK_AHB_DIV_MASK RCC_CFGR_HPRE_DIV64
#elif _CLK_AHB_DIV == 128
  #define _CLK_AHB_DIV_MASK RCC_CFGR_HPRE_DIV128
#elif _CLK_AHB_DIV == 256
  #define _CLK_AHB_DIV_MASK RCC_CFGR_HPRE_DIV256
#elif _CLK_AHB_DIV == 512
  #define _CLK_AHB_DIV_MASK RCC_CFGR_HPRE_DIV512
#else
  #error "_CLK_AHB_DIV out of range"
#endif
//------------------------------------------------------------
// _CLK_APB1_DIV
#ifndef _CLK_APB1_DIV
  #error "_CLK_APB1_DIV undefined"
#endif

#if   _CLK_APB1_DIV == 1
  #define _CLK_APB1_DIV_MASK RCC_CFGR_PPRE1_DIV1
#elif _CLK_APB1_DIV == 2
  #define _CLK_APB1_DIV_MASK RCC_CFGR_PPRE1_DIV2
#elif _CLK_APB1_DIV == 4
  #define _CLK_APB1_DIV_MASK RCC_CFGR_PPRE1_DIV4
#elif _CLK_APB1_DIV == 8
  #define _CLK_APB1_DIV_MASK RCC_CFGR_PPRE1_DIV8
#elif _CLK_APB1_DIV == 16
  #define _CLK_APB1_DIV_MASK RCC_CFGR_PPRE1_DIV16
#else
  #error "_CLK_APB1_DIV out of range"
#endif

#if _CLK_APB1_DIV  == 1
  #define _CLK_APB1_TIM_X 1
#else
  #define _CLK_APB1_TIM_X 2
#endif
//------------------------------------------------------------
// _CLK_APB2_DIV
#ifndef _CLK_APB2_DIV
  #error "_CLK_APB2_DIV undefined"
#endif

#if   _CLK_APB2_DIV == 1
  #define _CLK_APB2_DIV_MASK RCC_CFGR_PPRE2_DIV1
#elif _CLK_APB2_DIV == 2
  #define _CLK_APB2_DIV_MASK RCC_CFGR_PPRE2_DIV2
#elif _CLK_APB2_DIV == 4
  #define _CLK_APB2_DIV_MASK RCC_CFGR_PPRE2_DIV4
#elif _CLK_APB2_DIV == 8
  #define _CLK_APB2_DIV_MASK RCC_CFGR_PPRE2_DIV8
#elif _CLK_APB2_DIV == 16
  #define _CLK_APB2_DIV_MASK RCC_CFGR_PPRE2_DIV16
#else
  #error "_CLK_APB2_DIV out of range"
#endif

#if _CLK_APB2_DIV  == 1
  #define _CLK_APB2_TIM_X 1
#else
  #define _CLK_APB2_TIM_X 2
#endif

//------------------------------------------------------------
// _CLK_ADC_DIV
#ifndef _CLK_ADC_DIV
  #error "_CLK_ADC_DIV undefined"
#endif

#if   _CLK_ADC_DIV  == 1
  #define _CLK_ADC_DIV_MASK RCC_CFGR2_ADCPRE12_DIV1
#elif _CLK_ADC_DIV  == 2
  #define _CLK_ADC_DIV_MASK RCC_CFGR2_ADCPRE12_DIV2
#elif _CLK_ADC_DIV  == 4
  #define _CLK_ADC_DIV_MASK RCC_CFGR2_ADCPRE12_DIV4
#elif _CLK_ADC_DIV  == 6
  #define _CLK_ADC_DIV_MASK RCC_CFGR2_ADCPRE12_DIV6
#elif _CLK_ADC_DIV  == 8
  #define _CLK_ADC_DIV_MASK RCC_CFGR2_ADCPRE12_DIV8
#elif _CLK_ADC_DIV  == 10
  #define _CLK_ADC_DIV_MASK RCC_CFGR2_ADCPRE12_DIV10
#elif _CLK_ADC_DIV  == 12
  #define _CLK_ADC_DIV_MASK RCC_CFGR2_ADCPRE12_DIV12
#elif _CLK_ADC_DIV  == 16
  #define _CLK_ADC_DIV_MASK RCC_CFGR2_ADCPRE12_DIV16
#elif _CLK_ADC_DIV  == 32
  #define _CLK_ADC_DIV_MASK RCC_CFGR2_ADCPRE12_DIV32
#elif _CLK_ADC_DIV  == 64
  #define _CLK_ADC_DIV_MASK RCC_CFGR2_ADCPRE12_DIV64
#elif _CLK_ADC_DIV  == 128
  #define _CLK_ADC_DIV_MASK RCC_CFGR2_ADCPRE12_DIV128
#elif _CLK_ADC_DIV  == 256
  #define _CLK_ADC_DIV_MASK RCC_CFGR2_ADCPRE12_DIV256
#else
  #error "_CLK_ADC_DIV out of range"
#endif
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
// determine main OSC SPEED
#ifndef _OSC_SOURCE
  #error "_OSC_SOURCE undefined"
#endif

#if   _OSC_SOURCE == _OSC_HSI
    #define  _OSC_SPEED _INT_RC
    #define  _PLL_SPEED ( (_OSC_SPEED/2)*_PLL_MUL )
#elif _OSC_SOURCE == _OSC_HSE
    #define  _OSC_SPEED _CRYSTAL
    #define  _PLL_SPEED ( (_OSC_SPEED/_HSE_PLL_DIV)*_PLL_MUL )
#else
    #error "_OSC_SOURCE out of range"
#endif
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
// determine AHB SPEED
#ifndef _CLK_AHB_SOURCE
  #error "_CLK_AHB_SOURCE undefined"
#endif

#if   _CLK_AHB_SOURCE == _CLK_AHB_HSI
  #define _AHB_SPEED  (_OSC_SPEED/_CLK_AHB_DIV)
#elif _CLK_AHB_SOURCE == _CLK_AHB_HSE
  #define _AHB_SPEED  (_OSC_SPEED/_CLK_AHB_DIV)
#elif _CLK_AHB_SOURCE == _CLK_AHB_PLL
  #define _AHB_SPEED  (_PLL_SPEED/_CLK_AHB_DIV)
#else
  #error "_CLK_AHB_SOURCE out of range 1"
#endif
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
// determine APB1,APB2 SPEED
#define _APB1_SPEED (_AHB_SPEED/_CLK_APB1_DIV)
#define _APB2_SPEED (_AHB_SPEED/_CLK_APB2_DIV)
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
// system freq const
#define F_CPU _AHB_SPEED
#define F_AHB _AHB_SPEED
#define F_APB1 _APB1_SPEED
#define F_APB2 _APB2_SPEED

#define F_APB1_TIM (F_APB1*_CLK_APB1_TIM_X)
#define F_APB2_TIM (F_APB2*_CLK_APB2_TIM_X)
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
// clock system usage
#if   _CLK_AHB_SOURCE == _CLK_AHB_HSI

  #define _CLK_AHB_SOURCE_MASK   RCC_CFGR_SW_HSI

#elif _CLK_AHB_SOURCE == _CLK_AHB_HSE

  #define _CLK_HSE_USE
  #define _CLK_AHB_SOURCE_MASK   RCC_CFGR_SW_HSE

#elif _CLK_AHB_SOURCE == _CLK_AHB_PLL

  #define _CLK_PLL_USE
  #define _CLK_AHB_SOURCE_MASK   RCC_CFGR_SW_PLL

  #if _OSC_SOURCE == _OSC_HSI

    #define _PLL_OSC_MASK RCC_CFGR_PLLSRC*0

  #elif _OSC_SOURCE == _OSC_HSE

    #define _CLK_HSE_USE
    #define _PLL_OSC_MASK RCC_CFGR_PLLSRC*1

  #endif
#endif
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
#if F_CPU <= 24000000
    #define _CLK_WAIT_STATE 0
#endif

#if (F_CPU > 24000000) && (F_CPU <= 48000000)
    #define _CLK_WAIT_STATE 1
#endif

#if (F_CPU > 48000000) && (F_CPU <= 72000000)
    #define _CLK_WAIT_STATE 2
#endif

#if F_CPU > 72000000
    #error "F_CPU Too high!"
#endif

// flash options
#define flash_PAGE_SIZE   (2048)
// memory code-flash size
// B - 128k, 64  pages
// C - 256k, 128 pages
#define flash_PAGE_NUM    (64)   //
#define flash_SIZE        (flash_PAGE_NUM*flash_PAGE_SIZE)
//---------------------------------------------------------------------------

//-------------------------------------------------------------------------
#endif // _HK_SYS_DRV_303XC_H
//---------------------------------------------------------------------------