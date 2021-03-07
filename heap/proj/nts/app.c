
#include <stdio.h> 
#include "system.h" 
#include "pins.h"   
#include "gpio.h"   
//#include "spi.h"   
//#include "uart.h"   
//#include "term.h"   
#include "nts.h"   


#define _UART 3
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


#define ntsRxBufSize 32
#define ntsTxBufSize 32

#define nts485port gpioPA
#define nts485pin  11

void ledTask1(uint8_t th)
{

  pin_xor(ledPORT, led1);
}

void ledTask2(uint8_t th)
{
  pin_xor(ledPORT, led2);
}


void gpioInit(void)
{
    gpioEN_RCC(gpioPA);
    gpioEN_RCC(gpioPB);
    gpioEN_RCC(gpioPC);
	
    gpioCFG(ledPORT, led1, gpd_output | gpot_pushpull | gpos_hi | gpaf_no);  //
    gpioCFG(ledPORT, led2, gpd_output | gpot_pushpull | gpos_hi | gpaf_no);  //
	
//    gpioCFG(GPIOB, 0, gpd_input | gpit_pullup | gpaf_no);  //
//    gpioCFG(GPIOB, 1, gpd_input | gpit_pullup | gpaf_no);  //
//    gpioCFG(GPIOC, 6, gpd_input | gpit_pullup | gpaf_no);  //
//    gpioCFG(GPIOC, 7, gpd_input | gpit_pullup | gpaf_no);  //
	
//    gpioCFG(GPIOA, 8, gpd_output | gpot_pushpull | gpos_hi | gpaf_alter);  //
	
//#if _UART == 1
// USART1
  gpioCFG(GPIOA, 9 , gpd_output | gpot_pushpull | gpos_hi | gpaf_alter);  // TX
  gpioCFG(GPIOA, 10, gpd_input  | gpit_pullup   | 0       | gpaf_alter);  // RX
  gpioCFG(GPIOA, 11, gpd_output | gpot_pushpull | gpos_hi | gpaf_no   );  // DIR
//#endif	

//#if _UART == 2
// USART1
  gpioCFG(GPIOA, 2 , gpd_output | gpot_pushpull | gpos_hi | gpaf_alter);  // TX
  gpioCFG(GPIOA, 3, gpd_input  | gpit_pullup   | 0       | gpaf_alter);   // RX
//  gpioCFG(GPIOA, 11, gpd_output | gpot_pushpull | gpos_hi | gpaf_no   );  // DIR
//#endif	

//#if _UART == 3
// USART1
  gpioCFG(GPIOB, 10 , gpd_output | gpot_pushpull | gpos_hi | gpaf_alter);  // TX
  gpioCFG(GPIOB, 11, gpd_input  | gpit_pullup   | 0       | gpaf_alter);   // RX
//  gpioCFG(GPIOA, 11, gpd_output | gpot_pushpull | gpos_hi | gpaf_no   );  // DIR
//#endif	


#if _SPI == 1
//  SPI1
    gpioCFG(GPIOA, 5, gpd_output | gpot_pushpull | gpos_hi | gpaf_alter);  // spi sck
    gpioCFG(GPIOA, 6, gpd_input  | gpit_pullup   | 0       | gpaf_alter);  // spi miso
    gpioCFG(GPIOA, 7, gpd_output | gpot_pushpull | gpos_hi | gpaf_alter);  // spi mosi
		
//    gpioCFG(GPIOA, 4, gpd_output | gpot_pushpull | gpos_hi | gpaf_no);  //spi latch
#endif		

#if _SPI == 2
//  SPI2
  	gpioCFG(GPIOB, 13, gpd_output | gpot_pushpull | gpos_hi | gpaf_alter);  // spi sck
    gpioCFG(GPIOB, 14, gpd_input  | gpit_pullup   | 0       | gpaf_alter);  // spi miso
    gpioCFG(GPIOB, 15, gpd_output | gpot_pushpull | gpos_hi | gpaf_alter);  // spi mosi
		
//    gpioCFG(GPIOA, 4, gpd_output | gpot_pushpull | gpos_hi | gpaf_no);  //spi latch
#endif		


    gpioCFG(GPIOA, 2, gpd_output | gpot_pushpull | gpos_hi | gpaf_alter);  // spi sck

}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
/*
uint8_t test_rts_h;

void test_rts(void)
{
//	char a_str[40];
	uint32_t v;
	
	while (1)
	{
		while (scanf("%u",&v) != 1) {} ;
		printf("%s%X%s", "0x",v,"\r");
		
//		printf("%s", "Hello Word!\r");
//		rtsDelay(1000);
	  pin_xor(ledPORT, led2);
	}
}

void terminalInstall(void)
{
	test_rts_h = rtsAddTask(&test_rts, 1);
	terminalInit(test_rts_h);
}
*/

ntsType ntsh;
uint8_t ntsRxBuf[ntsRxBufSize];
uint8_t ntsTxBuf[ntsTxBufSize];

ntsType ntsh1;
uint8_t nts1RxBuf[ntsRxBufSize];
uint8_t nts1TxBuf[ntsTxBufSize];


void cm0Handler(void)
{
}

uint32_t pcnt = 0;

void cm7Handler(void)
{
	pcnt++;
	
	ntsTxBuf[2] = (pcnt >> 0) & 0xff;
	ntsTxBuf[3] = (pcnt >> 8) & 0xff;
	ntsTxBuf[4] = (pcnt >> 16) & 0xff;
	ntsTxBuf[5] = (pcnt >> 24) & 0xff;
	
	ntsh.txSize = 2 + 28;
	
  pin_xor(ledPORT, led2);
}

void c1m7Handler(void)
{
	pcnt++;
	
	nts1TxBuf[2] = (pcnt >> 0) & 0xff;
	nts1TxBuf[3] = (pcnt >> 8) & 0xff;
	nts1TxBuf[4] = (pcnt >> 16) & 0xff;
	nts1TxBuf[5] = (pcnt >> 24) & 0xff;
	
	ntsh1.txSize = 2 + 28;
	
  pin_xor(ledPORT, led2);
}



void ntsInstall(void)
{
	ntsh.port = uartPORT;
	ntsh.BaudeRate = 115200;
	ntsh.dirPort = nts485port;
	ntsh.dirPin  = nts485pin;
	ntsh.Addr = 1;
	ntsh.crcType = ntsCRC16;
	ntsh.Ack = 1;
	ntsh.sepTx = 0;
	ntsh.txBuffer = ntsTxBuf;
	ntsh.rxBuffer = ntsRxBuf;
	ntsh.rxBufSize = ntsRxBufSize;

	ntsAdd(&ntsh);
	
	ntsAddCm(&ntsh, 0, &cm0Handler);
	ntsAddCm(&ntsh, 7, &cm7Handler);
}

void nts1Install(void)
{
	ntsh1.port = USART2;
	ntsh1.BaudeRate = 115200;
	ntsh1.dirPort = nts485port;
	ntsh1.dirPin  = nts485pin;
	ntsh1.Addr = 1;
	ntsh1.crcType = ntsCRC16;
	ntsh1.Ack = 1;
	ntsh1.sepTx = 0;
	ntsh1.txBuffer = nts1TxBuf;
	ntsh1.rxBuffer = nts1RxBuf;
	ntsh1.rxBufSize = ntsRxBufSize;

	ntsAdd(&ntsh1);
	
//	ntsAddCm(&ntsh, 0, &cm0Handler);
	ntsAddCm(&ntsh1, 7, &c1m7Handler);
}


void perifInit(void)
{
  gpioInit();
//	spiInstall();
//	terminalInstall();
	ntsInstall();
	nts1Install();
}

void libInit(void)
{
}


uint8_t test_rts_h;
void appInit(void)
{
	stAddTask(&ledTask1,stf_10Hz);
	
	
//	stAddTask(&ledTask2,stf_10Hz);
//	stAddTask(&spiTask,stf_ring);
}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
