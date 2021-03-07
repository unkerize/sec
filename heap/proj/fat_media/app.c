
//#include <stdio.h> 
#include "system.h" 
#include "pins.h"   
#include "gpio.h"   
//#include "spi.h"   
//#include "uart.h"   
//#include "ff.h"   

#include "app_fat.h"   

#define _UART 2
#define _SPI  2

#if    _SPI == 1
  #define spiPORT SPI1
#elif  _SPI == 2
  #define spiPORT SPI2
#endif	

#if    _UART == 1
  #define uartPORT USART1
#elif  _UART == 2
  #define uartPORT USART2
#elif  _UART == 3
  #define uartPORT USART3
#endif	



void ledTask1(uint8_t th)
{
  pin_xor(ledPORT, blinkLED);
}

void gpioInit(void)
{
    gpioEN_RCC(gpioPA);
    gpioEN_RCC(gpioPB);
    gpioEN_RCC(gpioPC);
    gpioEN_RCC(gpioPE);
	
    gpioCFG(ledPORT, led1, gpd_output | gpot_pushpull | gpos_hi | gpaf_no);  //
    gpioCFG(ledPORT, led2, gpd_output | gpot_pushpull | gpos_hi | gpaf_no);  //
    gpioCFG(ledPORT, led3, gpd_output | gpot_pushpull | gpos_hi | gpaf_no);  //
    gpioCFG(ledPORT, led4, gpd_output | gpot_pushpull | gpos_hi | gpaf_no);  //
    gpioCFG(ledPORT, led5, gpd_output | gpot_pushpull | gpos_hi | gpaf_no);  //
    gpioCFG(ledPORT, led6, gpd_output | gpot_pushpull | gpos_hi | gpaf_no);  //
    gpioCFG(ledPORT, led7, gpd_output | gpot_pushpull | gpos_hi | gpaf_no);  //
    gpioCFG(ledPORT, led8, gpd_output | gpot_pushpull | gpos_hi | gpaf_no);  //
//	  pin_set(ledPORT, led1);
	
    gpioCFG(GPIOA, 4 , gpd_analog | gpot_pushpull | gpos_hi | gpaf_no);  // DAC1
    gpioCFG(GPIOA, 5 , gpd_analog | gpot_pushpull | gpos_hi | gpaf_no);  // DAC2
//    gpioCFG(GPIOA, 4 , gpd_output | gpot_pushpull | gpos_hi | gpaf_no);  // DAC1
//    gpioCFG(GPIOA, 5 , gpd_output | gpot_pushpull | gpos_hi | gpaf_no);  // DAC2
	
//	  pin_set(ledPORT, led2);
	
//    gpioCFG(GPIOB, 0, gpd_input | gpit_pullup | gpaf_no);  //
//    gpioCFG(GPIOB, 1, gpd_input | gpit_pullup | gpaf_no);  //
//    gpioCFG(GPIOC, 6, gpd_input | gpit_pullup | gpaf_no);  //
//    gpioCFG(GPIOC, 7, gpd_input | gpit_pullup | gpaf_no);  //
	
//    gpioCFG(GPIOA, 8, gpd_output | gpot_pushpull | gpos_hi | gpaf_alter);  //

	
#if _UART == 1
// USART1
  gpioCFG(GPIOA, 9 , gpd_output | gpot_pushpull | gpos_hi | gpaf_alter);  // TX
  gpioCFG(GPIOA, 10, gpd_input  | gpit_pullup   | 0       | gpaf_alter);  // RX
  gpioCFG(GPIOA, 11, gpd_output | gpot_pushpull | gpos_hi | gpaf_no   );  // DIR
#endif	

#if _UART == 2
// USART1
  gpioCFG(GPIOA, 2 , gpd_output | gpot_pushpull | gpos_hi | gpaf_alter | (7<<gpaf_indx_bp) );  // TX
  gpioCFG(GPIOA, 3, gpd_input  | gpit_pullup   | 0       | gpaf_alter  | (7<<gpaf_indx_bp) );   // RX
  gpioCFG(GPIOA, 11, gpd_output | gpot_pushpull | gpos_hi | gpaf_no   );  // DIR
#endif	

#if _UART == 3
// USART1
  gpioCFG(GPIOB, 10 , gpd_output | gpot_pushpull | gpos_hi | gpaf_alter);  // TX
  gpioCFG(GPIOB, 11, gpd_input  | gpit_pullup   | 0       | gpaf_alter);   // RX
  gpioCFG(GPIOA, 11, gpd_output | gpot_pushpull | gpos_hi | gpaf_no   );  // DIR
#endif	


#if _SPI == 1
//  SPI1
    gpioCFG(GPIOA, 5, gpd_output | gpot_pushpull | gpos_hi | gpaf_alter);  // spi sck
    gpioCFG(GPIOA, 6, gpd_input  | gpit_pullup   | 0       | gpaf_alter);  // spi miso
    gpioCFG(GPIOA, 7, gpd_output | gpot_pushpull | gpos_hi | gpaf_alter);  // spi mosi
		
    gpioCFG(GPIOB, 12, gpd_output | gpot_pushpull | gpos_hi | gpaf_no);  //spi latch
#endif		

#if _SPI == 2
//  SPI2
  	gpioCFG(GPIOB, 13, gpd_output | gpot_pushpull | gpos_hi | gpaf_alter | (5<<gpaf_indx_bp) );  // spi sck
    gpioCFG(GPIOB, 14, gpd_input  | gpit_pullup   | 0       | gpaf_alter | (5<<gpaf_indx_bp) );  // spi miso
    gpioCFG(GPIOB, 15, gpd_output | gpot_pushpull | gpos_hi | gpaf_alter | (5<<gpaf_indx_bp) );  // spi mosi
		
    gpioCFG(GPIOB, 12, gpd_output | gpot_pushpull | gpos_hi | gpaf_no);  //spi latch sd sel
#endif		

   pin_set(GPIOB, 12);
	 
// SPI3	 
  	gpioCFG(GPIOC, 10, gpd_output | gpot_pushpull | gpos_hi | gpaf_alter | (6<<gpaf_indx_bp) );  // spi sck
    gpioCFG(GPIOC, 11, gpd_input  | gpit_pullup   | 0       | gpaf_alter | (6<<gpaf_indx_bp) );  // spi miso
    gpioCFG(GPIOC, 12, gpd_output | gpot_pushpull | gpos_hi | gpaf_alter | (6<<gpaf_indx_bp) );  // spi mosi
	 
	 
}


void perifInit(void)
{
  gpioInit();
}

void libInit(void)
{
}

void appInit(void)
{
	stAddTask(&ledTask1,stf_1kHz);
	
//	mediaInit(mediaBuf1, mediaBuf2);
	fat_init();

}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
