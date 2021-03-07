//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
#ifndef _HK_PRJ_DISCOVERY_H
#define _HK_PRJ_DISCOVERY_H
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// project
#define BRD_DISCOVERY_VL

// CPU
//#define STM32F303xC
//-----------------------------------------------------------------------------
// pins & leds
#define ledPORT GPIOE

#define led1 8  // working led blink
#define led2 9  // fat error

#define led3 10  // working led blink
#define led4 11  // rs led
#define led5 12  // working led blink
#define led6 13  // rs led
#define led7 14  // working led blink
#define led8 15  // rs led

#define blinkLED  led1
#define sampleLED led2
#define block_readLED  led3
#define errorLED  led4
#define kbLED     led5
#define fatLED    led6
#define block_playLED  led7
//#define blinkLED led1
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
#endif //_HK_PRJ_DISCOVERY_H
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
