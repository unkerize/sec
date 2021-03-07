//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
#ifndef _HK_PRJ_XX
#define _HK_PRJ_XX
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// CPU
//  KEIL DONT insert define
#define STM32F10X_MD
//-----------------------------------------------------------------------------
// pins & leds
  #define led1PORT GPIOA
  #define led1 5   // working led blink

  #define led2PORT GPIOA
  #define led2 6  // rs led

  #define led3PORT GPIOA
  #define led3 7  // active led

  #define led4PORT GPIOB
  #define led4 0  // fail led

  #define tlEnPort GPIOB
  #define tlEnPin  7  // EN = 1

  #define extEnPort GPIOA         // opto-in
  #define extEnPin  2  // EN = 0

  #define coolPORT GPIOA
  #define coolPIN  3    // act = 1

  #define readyPORT GPIOA
  #define readyPIN  4    // act = 0

  #define failPORT GPIOB
  #define failPIN  1    // act = 0

  #define fanPORT GPIOB
  #define fanPIN  6    // act = 0

//  #define fail1PORT GPIOA
//  #define fail1PIN  10    // act = 0

//  #define fail2PORT GPIOA
//  #define fail2PIN  11    // act = 0
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
#endif //_HK_PRJ_XX
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
