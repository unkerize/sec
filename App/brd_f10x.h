//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
#ifndef _HK_PRJ_XX
#define _HK_PRJ_XX
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// CPU
//  KEIL insert define
//#define STM32F030
//-----------------------------------------------------------------------------
// pins & leds
#define led1Port GPIOC
#define led1Pin  8  // working led blink

#define led2Port GPIOC
#define led2Pin  9  //

// Analog inputs
#define anVoltagePort GPIOA
#define anVoltagePin 0  //

#define anCurrentPort GPIOA
#define anCurrentPin 1  //

// comm interface
#define commTxPort   GPIOA  //
#define commTxPin    2      //
#define commTxAlterF 1      //

#define commRxPort   GPIOA  //
#define commRxPin    3      //
#define commRxAlterF 1      //

// reg mos output
#define regEnPort  GPIOA  //
#define regEnPin   9      //

#define regPwmPort GPIOA  //
#define regPwmPin  10     //
#define regAlterF  2      //

//#define STM32F10X
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
#endif //_HK_PRJ_XX
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
