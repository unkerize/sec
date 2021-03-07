



#ifndef _HK_rs_stm32f100RB
#define _HK_rs_stm32f100RB

#include <stdint.h>
#include "..\..\..\core\system.h"
#include "..\..\..\libs\pins.h"

#include "..\..\..\perif\rs\rscfg.h"
#include "..\..\..\perif\rs\nts.h"


#define rsBaudDiv (F_CPU/16/rsBaudRate)

#if   rsPORT == 1
  #define _rsmPORT USART1

  #define _rsm_TX_DMA DMA1_Channel4
  #define _rsm_RX_DMA DMA1_Channel5

  #define _rsm_APB_EN   APB2ENR
  #define _rsm_APB_MASK RCC_APB2ENR_USART1EN

  #define _rsm_IRQ USART1_IRQn
  #define _rsm_RX_Vector USART1_IRQHandler

#elif rsPORT == 2
  #define _rsmPORT USART2

  #define _rsm_TX_DMA DMA1_Channel7
  #define _rsm_RX_DMA DMA1_Channel6

  #define _rsm_APB_EN   APB1ENR
  #define _rsm_APB_MASK RCC_APB1ENR_USART2EN

  #define _rsm_IRQ USART2_IRQn
  #define _rsm_RX_Vector USART2_IRQHandler

#elif rsPORT == 3
  #define _rsmPORT USART3

  #define _rsm_TX_DMA DMA1_Channel2
  #define _rsm_RX_DMA DMA1_Channel3

  #define _rsm_APB_EN   APB1ENR
  #define _rsm_APB_MASK RCC_APB1ENR_USART3EN

  #define _rsm_IRQ USART3_IRQn
  #define _rsm_RX_Vector USART3_IRQHandler

#endif

void rsInit(ntsData_t* lp_rs);
void rsSendPack(uint8_t lSize);
uint8_t rsTxBusy(void);

#endif // _HK_rs_stm32f100RB
