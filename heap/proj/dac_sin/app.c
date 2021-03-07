
#include <stdio.h>
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
    gpioEN_RCC(gpioPE);

    gpioCFG(ledPORT, led1, gpd_output | gpot_pushpull | gpos_hi | gpaf_no);  //
    gpioCFG(ledPORT, led2, gpd_output | gpot_pushpull | gpos_hi | gpaf_no);  //
    gpioCFG(ledPORT, led3, gpd_output | gpot_pushpull | gpos_hi | gpaf_no);  //
    gpioCFG(ledPORT, led4, gpd_output | gpot_pushpull | gpos_hi | gpaf_no);  //
    gpioCFG(ledPORT, led5, gpd_output | gpot_pushpull | gpos_hi | gpaf_no);  //
    gpioCFG(ledPORT, led6, gpd_output | gpot_pushpull | gpos_hi | gpaf_no);  //
    gpioCFG(ledPORT, led7, gpd_output | gpot_pushpull | gpos_hi | gpaf_no);  //
    gpioCFG(ledPORT, led8, gpd_output | gpot_pushpull | gpos_hi | gpaf_no);  //
          pin_set(ledPORT, led1);

    gpioCFG(GPIOA, 4 , gpd_analog | gpot_pushpull | gpos_hi | gpaf_no);  // TX

//        pin_set(ledPORT, led2);

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
}



//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
uint8_t spih1;
uint8_t spi_task_h;

void spi_task(void)
{
//      uint8_t temp;
        while(1)
        {
//               temp = spiTxByteAsync(spih1, temp);
                rtsDelay(10);

        }
//      spiTxByte(spih1, 0x81);
//      *(__IO uint8_t *)&SPI2->DR = 0x1;
//      SPI2->DR = 0x00;
//      (uint8_t) SPI2->DR = 0x00;
}

#define timF   44100
#define timPSC 64
#define timARR (F_APB1_TIM/timPSC/timF)

void TIM6_DAC_IRQHandler(void)
{
  if ( (TIM6->SR & TIM_SR_UIF) == TIM_SR_UIF )
        {
                TIM6->SR &= ~TIM_SR_UIF;

                pin_xor(ledPORT,led3);
        }

}

//#define F_APB1_TIM (F_APB1*_CLK_APB1_TIM_X)
//#define F_APB2_TIM (F_APB2*_CLK_APB2_TIM_X)


#define dacBufferSize 0x100
uint16_t dacBuffer[dacBufferSize];

const uint32_t sinTab[16] = {0, 980,1951, 2903, 3827, 4714, 5556, 6344, 7071, 7730, 8315, 8819, 9239, 9669, 9808, 9952};

void tim6Init(void)
{
        int16_t i,j = 0;

        for (i = 0; i < dacBufferSize; )
        {
                for (j = 0; j < 16; j++) dacBuffer[i++] = 2047 + sinTab[j]*2047/10000;
                for (j = 15; j >= 0; j--) dacBuffer[i++] = 2047 + sinTab[j]*2047/10000; //(10000 - sinTab[j])*2048/10000;
                for (j = 0; j < 16; j++) dacBuffer[i++] = 2047 - sinTab[j]*2047/10000;
                for (j = 15; j >= 0; j--) dacBuffer[i++] = 2047 - sinTab[j]*2047/10000; //(10000 - sinTab[j])*2048/10000;
        }


//      dacBuffer[i] = i << 4;


  RCC->AHBENR  |= RCC_AHBENR_DMA2EN;
  RCC->APB1ENR |= RCC_APB1ENR_DAC1EN;
  RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;


        DMA2_Channel3->CCR = DMA_CCR_MINC |  DMA_CCR_PSIZE_0*0x2 | DMA_CCR_MSIZE_0*0x1 | DMA_CCR_DIR  | DMA_CCR_CIRC; // | DMA_CCR_EN;
        DMA2_Channel3->CPAR = (uint32_t) &DAC->DHR12R1;
        DMA2_Channel3->CMAR = (uint32_t) &dacBuffer;
        DMA2_Channel3->CNDTR = dacBufferSize;


//      DAC->CR = 0x00*DAC_CR_TSEL1_0 | DAC_CR_TEN1 | DAC_CR_EN1 | DAC_CR_DMAEN1;
        DAC->CR = 0x00*DAC_CR_TSEL1_0 | DAC_CR_TEN1 | DAC_CR_EN1 | DAC_CR_DMAEN1;

        TIM6->PSC  = timPSC;
        TIM6->ARR  = timARR;
        TIM6->DIER = TIM_DIER_UIE;

        NVIC_EnableIRQ(TIM6_DAC_IRQn);

        TIM6->CR2  = 0x02*TIM_CR2_MMS_0;

        TIM6->CR1  = TIM_CR1_URS | TIM_CR1_CEN;
  DMA2_Channel3->CCR |= DMA_CCR_EN;
}



void perifInit(void)
{
//   RCC->AHBENR |= RCC_AHBENR_GPIOEEN;
//       GPIOE->MODER |= (1<< (8*2)) | (1<<(9*2));

//        pin_set(GPIOE,8);

  gpioInit();
        tim6Init();

// spih1 =  spiAdd(SPI2, spioMODE0 | spioMSBF | spioBR4 | 0 | (0<<spioOSindx_bp) );

//      spiInstall();
//      terminalInstall();
}

void libInit(void)
{
}

void appInit(void)
{
        stAddTask(&ledTask1,stf_10Hz);

//  spi_task_h = rtsAddTask(&spi_task, 1);
//  spih1 =  spiAdd(SPI2, spioMODE0 | spioMSBF | spioBR4 | spioOsRTS | (spi_task_h<<spioOSindx_bp) );


//      stAddTask(&spi_task,stf_10Hz);

        fat_add();


//      stAddTask(&ledTask2,stf_10Hz);
//      stAddTask(&spiTask,stf_ring);

//      fat_work();

}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
