

#include "system.h" 
#include "pins.h"   
#include "gpio.h"   

#include <stdlib.h> 
#include <string.h> 
//#include <math.h> 

//#include "adc.h"   

//#include <stdio.h> 
//#include "kbkey.h"   
//#include "din.h"   
//#include "spi.h"   
//#include "uart.h"   
//#include "nts.h"   

//#include "tim.h"   
//#include "ds.h"   
//#include "flash.h"

void init_f030(void)
{
// SPI-UART DMA remap
//  RCC->APB2ENR |= RCC_APB2ENR_SYSCFGCOMPEN;
//  SYSCFG->CFGR1 |= SYSCFG_CFGR1_USART1TX_DMA_RMP | SYSCFG_CFGR1_USART1RX_DMA_RMP;
}


void gpioInit(void)
{
//  init_f030();
//----------------------------------------  
    gpioEN_RCC(GPIOA);
    gpioEN_RCC(GPIOB);
    gpioEN_RCC(GPIOC);
//    gpioEN_RCC(GPIOF);

//    gpioEN_RCC(GPIOD);
//    gpioEN_RCC(GPIOE);
//----------------------------------------  
// Leds
//  gpioCFG(GPIOE, 8, gpd_output | gpot_pushpull | gpos_hi | gpaf_no);  //
//  gpioCFG(GPIOE, 9, gpd_output | gpot_pushpull | gpos_hi | gpaf_no);  //
//  gpioCFG(GPIOE, 10, gpd_output | gpot_pushpull | gpos_hi | gpaf_no);  //
  gpioCFG(GPIOC, 8, gpd_output | gpot_pushpull | gpos_hi | gpaf_no);  //
  gpioCFG(GPIOC, 9, gpd_output | gpot_pushpull | gpos_hi | gpaf_no);  //
	
//  gpioCFG(GPIOA, 15, gpd_output | gpot_pushpull | gpos_hi | gpaf_alter | 1<<gpaf_indx_bp  ); //TX
	
//  pin_set(GPIOA, 5);
//	while (1) {};

	
//  gpioCFG(GPIOE, 9, gpd_output | gpot_pushpull | gpos_hi | gpaf_no);  //
//  gpioCFG(GPIOE, 10, gpd_output | gpot_pushpull | gpos_hi | gpaf_no);  //
//  gpioCFG(GPIOE, 11, gpd_output | gpot_pushpull | gpos_hi | gpaf_no);  //
//  pin_set(GPIOE, 8);
  
//  gpioCFG(GPIOF, 1,  gpd_output | gpot_pushpull | gpos_hi | gpaf_no    | 0 );  //IO2
//  pin_set(GPIOF, 1);
//----------------------------------------  
// ADC / analog
//  gpioCFG(GPIOA, 0 , gpd_analog | gpot_pushpull | gpos_hi | gpaf_no);  // ADC1
//  gpioCFG(GPIOA, 1 , gpd_analog | gpot_pushpull | gpos_hi | gpaf_no);  // ADC2
//  gpioCFG(GPIOA, 4 , gpd_analog | gpot_pushpull | gpos_hi | gpaf_no);  // 
//  gpioCFG(GPIOA, 5 , gpd_analog | gpot_pushpull | gpos_hi | gpaf_no);  // 

//  gpioCFG(GPIOC, 0 , gpd_analog | gpot_pushpull | gpos_hi | gpaf_no);  // 
//  gpioCFG(GPIOC, 1 , gpd_analog | gpot_pushpull | gpos_hi | gpaf_no);  //

//  gpioCFG(GPIOD, 10 , gpd_analog | gpot_pushpull | gpos_hi | gpaf_no);  // 
//  gpioCFG(GPIOD, 11 , gpd_analog | gpot_pushpull | gpos_hi | gpaf_no);  //
  gpioCFG(GPIOA, 5 , gpd_analog | gpot_pushpull | gpos_hi | gpaf_no);  // DAC1_CH2 out
  gpioCFG(GPIOA, 6 , gpd_analog | gpot_pushpull | gpos_hi | gpaf_no);  // DAC2_CH1 out
  gpioCFG(GPIOA, 7 , gpd_analog | gpot_pushpull | gpos_hi | gpaf_no);  // COMP2+ in
  gpioCFG(GPIOB, 0 , gpd_analog | gpot_pushpull | gpos_hi | gpaf_no);  // COMP4+ in
//  pin_set(GPIOB, 8);
//----------------------------------------  
// Outputs
//  gpioCFG(GPIOB, 15,  gpd_output | gpot_pushpull | gpos_hi | gpaf_no  );  //
//  pin_set(GPIOB, 15);
//----------------------------------------  
// Inputs
//  gpioCFG(GPIOA, 13, gpd_input  | gpit_pullup | 0 | gpaf_no);  //
//  gpioCFG(GPIOA, 14, gpd_input  | gpit_pullup | 0 | gpaf_no);  //

//----------------------------------------  
// TIM1, CH 1, 2, 3, 4
//  gpioCFG(GPIOA, 8,   gpd_output | gpot_pushpull | gpos_hi | gpaf_alter | 2<<gpaf_indx_bp );  //
//  gpioCFG(GPIOA, 9,   gpd_output | gpot_pushpull | gpos_hi | gpaf_alter | 2<<gpaf_indx_bp );  //
//  gpioCFG(GPIOA, 10,  gpd_output | gpot_pushpull | gpos_hi | gpaf_alter | 2<<gpaf_indx_bp );  //
//  gpioCFG(GPIOA, 11,  gpd_output | gpot_pushpull | gpos_hi | gpaf_alter | 11<<gpaf_indx_bp );  //

//----------------------------------------  
// TIM3
//  gpioCFG(GPIOB, 1,  gpd_output | gpot_pushpull | gpos_hi | gpaf_alter | 1<<gpaf_indx_bp );  //C4
//  gpioCFG(GPIOC, 6,  gpd_output | gpot_pushpull | gpos_hi | gpaf_alter | 0<<gpaf_indx_bp );  //
//  gpioCFG(GPIOC, 7,  gpd_output | gpot_pushpull | gpos_hi | gpaf_alter | 0<<gpaf_indx_bp );  //

// TIM14, 15, 16, 17 PMW
//  gpioCFG(GPIOA, 4,  gpd_output | gpot_pushpull | gpos_hi | gpaf_alter | 4<<gpaf_indx_bp );  //
//  gpioCFG(GPIOA, 6,  gpd_output | gpot_pushpull | gpos_hi | gpaf_alter | 5<<gpaf_indx_bp );  //
//  gpioCFG(GPIOA, 7,  gpd_output | gpot_pushpull | gpos_hi | gpaf_alter | 5<<gpaf_indx_bp );  //
//  gpioCFG(GPIOC, 9,  gpd_output | gpot_pushpull | gpos_hi | gpaf_alter | 0<<gpaf_indx_bp );  //

//  gpioCFG(GPIOB, 14, gpd_input  | gpit_pullup             | gpaf_alter | 1<<gpaf_indx_bp );  // T15C1
//  gpioCFG(GPIOB, 15, gpd_input  | gpit_pullup             | gpaf_alter | 1<<gpaf_indx_bp );  // T15C2
  
//  gpioCFG(GPIOB, 4, gpd_input  | gpit_pullup             | gpaf_alter | 1<<gpaf_indx_bp );  //T16C1
//  gpioCFG(GPIOB, 5, gpd_input  | gpit_pullup             | gpaf_alter | 10<<gpaf_indx_bp );  //T17C1
	
	
//----------------------------------------  
// Uart inreface 1
//  gpioCFG(GPIOA, 9,  gpd_output | gpot_pushpull | gpos_hi | gpaf_alter | 7<<gpaf_indx_bp );  //TX F030 AF = 1
//  gpioCFG(GPIOA, 10, gpd_input  | gpit_pullup             | gpaf_alter | 7<<gpaf_indx_bp );  //RX F303 AF = 7
//  gpioCFG(GPIOA, 12, gpd_output | gpot_pushpull | gpos_hi | gpaf_no);  //
//  pin_clr(GPIOA, 12);

//----------------------------------------  
// Uart inreface 3
//  gpioCFG(GPIOB, 10, gpd_output | gpot_pushpull | gpos_hi | gpaf_alter | 7<<gpaf_indx_bp  ); //TX
//  gpioCFG(GPIOB, 11, gpd_input  | gpit_pullup             | gpaf_alter | 7<<gpaf_indx_bp );  //RX
//  gpioCFG(GPIOA, 8,  gpd_output | gpot_pushpull | gpos_hi | gpaf_no    | 0 );  //dir pin

//----------------------------------------  
// SPI1
//  gpioCFG(GPIOA, 5, gpd_output | gpot_pushpull | gpos_hi | gpaf_alter | 0<<gpaf_indx_bp );  // spi sck
//  gpioCFG(GPIOA, 6, gpd_input  | gpit_pullup   | 0       | gpaf_alter | 0<<gpaf_indx_bp );  // spi miso
//  gpioCFG(GPIOA, 7, gpd_output | gpot_pushpull | gpos_hi | gpaf_alter | 0<<gpaf_indx_bp );  // spi mosi
//  gpioCFG(GPIOA, 4,  gpd_output | gpot_pushpull | gpos_hi | gpaf_no    | 0 );  //CS pin
//  pin_set(GPIOA, 4);

//----------------------------------------  
// SPI2
//  gpioCFG(GPIOB, 13, gpd_output | gpot_pushpull | gpos_hi | gpaf_alter | 5<<gpaf_indx_bp );  // spi sck
//  gpioCFG(GPIOB, 14, gpd_input  | gpit_pullup   | 0       | gpaf_alter | 5<<gpaf_indx_bp );  // spi miso
//  gpioCFG(GPIOB, 15, gpd_output | gpot_pushpull | gpos_hi | gpaf_alter | 5<<gpaf_indx_bp );  // spi mosi

//----------------------------------------  
// SPI3
//  gpioCFG(GPIOC, 10, gpd_output | gpot_pushpull | gpos_hi | gpaf_alter | 6<<gpaf_indx_bp );  // spi sck
//  gpioCFG(GPIOC, 11, gpd_input  | gpit_pullup   | 0       | gpaf_alter | 6<<gpaf_indx_bp );  // spi miso
//  gpioCFG(GPIOC, 12, gpd_output | gpot_pushpull | gpos_hi | gpaf_alter | 6<<gpaf_indx_bp );  // spi mosi
//  gpioCFG(ltc_port, ltc_pin, gpd_output | gpot_pushpull | gpos_hi | gpaf_no );  // PWM
//  pin_clr(ltc_port, ltc_pin);

//----------------------------------------  
}
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
uint32_t a, b, c, d, e, f, g;

void ledTask1(void)
{
//  pin_set(GPIOE, 8);
	a = b+1;
	c = a+1;
	d = c+1;
	e = d+1;
	f = e+1;
	g = f+1;
//  pin_set(GPIOE, 8);
//  pin_set(GPIOE, 8);
//  pin_set(GPIOE, 8);
//  pin_set(GPIOE, 8);
//  pin_set(GPIOE, 8);
//  pin_set(GPIOE, 8);
//  pin_set(GPIOE, 8);
//  pin_clr(GPIOE, 8);
}

void ledTask2(void)
{
  pin_xor(GPIOC, 8);
//  pin_xor(led2Port, led2Pin);
}

void ledTask3(void)
{
  pin_xor(GPIOC, 9);
}

void ledTask4(void)
{
//  pin_xor(led4Port, led4Pin);
}

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

void appInit(void)
{
//  SCB->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;// ????????? ???????????? DWT
//  DWT_CONTROL|= DWT_CTRL_CYCCNTENA_Msk; // ???????? ???????
//  DWT_CYCCNT = 0;	
  // perif init
  gpioInit();
	
//	hr_tim_init();

  
//  clk_tim_init();
//  cnt_tim_init();
//  tims_init();

// adc_buff = adc_add(ADC1, (uint8_t*) adc_channels, ADC_CHAN_NUM, 12, 0, 0 , 0x07); //&ledPulse1
//  spi_inst = spiAdd(SPI1, spioOsNON | spioMODE1 | spioBR4 | spioMSBF | spioOsST | (spi_CS_inst <<spioOSindx_bp) );
//  spi_CS_inst = stAddTask(&spi_CS_deassert,stf_event);
  
  // libs/units init
//  flashInit(&cfg, 0, sizeof(cfg), 0);
//  update_cfg();
  
//  conn_ds();
//  conn_uart();
  
//	while (1) {};
  
  
  
//  stAddTask(&ledTask1,stf_1kHz);
//  stAddTask(&comp2_monitor	,stf_ring);
	


  stAddTask(&ledTask2,stf_10Hz);
  stAddTask(&ledTask3,stf_10Hz);
//  t3 = stAddTask(&ledTask3,stf_10Hz);
//  t4 = stAddTask(&ledTask4,stf_10Hz);
  
//  stSleepTask(t2);
//  stSleepTask(t3);
  
//  stAddTask(&pi_reg,stf_1kHz);
//  stAddTask(&tep_reg,stf_10Hz);
  
//  stAddTask(&comm_blink_handler,stf_1kHz);
//  stAddTask(&spi_tick,stf_1kHz);

}



void sysErrHandler(uint32_t err)
{
//  if (err == ser_dma_cross) 
//		pin_set(led2Port, led2Pin);
//	  pin_clr(led2Port, led2Pin);
	
	while (1) {};
}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
