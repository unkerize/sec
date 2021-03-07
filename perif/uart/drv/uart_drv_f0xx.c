
#include "system.h"
#include "uart.h"
#include "pins.h"
#include "gpio.h"

#define uartMaxNumber 2

#define uartTxDmaBlockMask (DMA_CCR_MINC |  DMA_CCR_PSIZE_1 | DMA_CCR_DIR  | DMA_CCR_EN)
#define uartRxDmaBlockMask (DMA_CCR_MINC |  DMA_CCR_PSIZE_1 | 0            | DMA_CCR_EN)

typedef struct {
  uint8_t  osRxOpt;
  uint8_t  osRxIndex;
  uint8_t  osTxOpt;
  uint8_t  osTxIndex;

  USART_TypeDef*        port;
  DMA_Channel_TypeDef*  txDMA;
  DMA_Channel_TypeDef*  rxDMA;

  GPIO_TypeDef*         rs485port;
  uint8_t               rs485pin;

  uint16_t              rxSize;
  uint16_t              rxBufSize;
  uint8_t*              rxBuffer;

  uint8_t               txBusy;
  uint8_t               rxReady;
} uartType;

uint8_t uartCounter = 0;

uartType uart[uartNumber];


uint8_t uart_handler[uartMaxNumber];

void uartSetRxEv(uint8_t luart)
{
  if (uart[luart].osRxOpt == uartOsNON) return;

  if (uart[luart].osRxOpt == uartOsST )
  {
    stSetEvent(uart[luart].osRxIndex);
    return;
  }

  if (uart[luart].osRxOpt == uartOsRTS)
  {
    rtsSetEvent(uart[luart].osRxIndex);
    return;
  }
}

void uartSetTxEv(uint8_t luart)
{
  if (uart[luart].osTxOpt == uartOsNON) return;

  if (uart[luart].osTxOpt == uartOsST )
  {
    stSetEvent(uart[luart].osTxIndex);
    return;
  }

  if (uart[luart].osTxOpt == uartOsRTS)
  {
    rtsSetEvent(uart[luart].osTxIndex);
    return;
  }
}

uint8_t uartTxBusy(uint8_t luart)
{
  return uart[luart].txBusy;
}

uint8_t uartRxReady(uint8_t luart)
{
  return uart[luart].rxReady;
}


uint16_t uartGetRxSize(uint8_t luart)
{
  return uart[luart].rxSize;
}

void uartTxBlock(uint8_t luart, uint8_t* ltxBuf, uint16_t ltxSize)
{
  if (ltxSize == 0) return;

  if (uart[luart].rs485port)
    pin_set(uart[luart].rs485port, uart[luart].rs485pin);

  uart[luart].txBusy  = 1;

  uart[luart].txDMA->CCR   = 0;
  uart[luart].txDMA->CNDTR = ltxSize;
  uart[luart].txDMA->CMAR  = (uint32_t) ltxBuf;
  uart[luart].txDMA->CCR   = uartTxDmaBlockMask;
  uart[luart].port->CR1   |= USART_CR1_TCIE;
}

void uartRxStop(uint8_t luart)
{
  uart[luart].rxSize = uart[luart].rxBufSize - uart[luart].rxDMA->CNDTR;
  uart[luart].rxDMA->CCR   = 0;
}

void uartResetRx(uint8_t luart)
{
//  uartDrv[luart].port->SR;
//  uartDrv[luart].port->DR;
  uart[luart].rxDMA->CNDTR = (uint32_t) uart[luart].rxBufSize;
  uart[luart].rxDMA->CMAR  = (uint32_t) uart[luart].rxBuffer;
  uart[luart].rxDMA->CCR   = uartRxDmaBlockMask;

  uart[luart].rxReady = 0;
}

void uartSetBaudeRate(uint8_t luart, uint32_t lBaudeRate)
{
  uint32_t uartBaudDiv;

  if (uart[luart].port == USART1) uartBaudDiv = F_APB2;
    else uartBaudDiv = F_APB1;

  uart[luart].port->BRR = (uartBaudDiv/lBaudeRate);
}

uint8_t uartAdd(uartCfgType* lCfg)
{
  IRQn_Type  uartIRQ;
  uint32_t   uartBaudDiv;

  uart[uartCounter].osRxOpt = lCfg->osRxFlag;
  uart[uartCounter].osTxOpt = lCfg->osTxFlag;

  uart[uartCounter].osRxIndex = lCfg->osRxIndex;
  uart[uartCounter].osTxIndex = lCfg->osTxIndex;;

  uart[uartCounter].rs485port = lCfg->Port485;
  uart[uartCounter].rs485pin  = lCfg->Pin485;

  if (uart[uartCounter].rs485port)
    pin_clr(uart[uartCounter].rs485port, uart[uartCounter].rs485pin);

  uart[uartCounter].port = lCfg->Port;

  uart[uartCounter].rxSize = 0;

  uart[uartCounter].rxBuffer  = lCfg->rxBuf;
  uart[uartCounter].rxBufSize = lCfg->rxBufSize;

  uart[uartCounter].txBusy  = 0;
  uart[uartCounter].rxReady = 0;

  if (uart[uartCounter].port == USART1)
  {
    uart_handler[0] = uartCounter;

    uartIRQ     = USART1_IRQn;

    uart[uartCounter].txDMA = DMA1_Channel4;
    uart[uartCounter].rxDMA = DMA1_Channel5;

    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
    RCC->AHBENR  |= RCC_AHBENR_DMA1EN;

    uartBaudDiv = F_APB2;
  }

#ifdef RCC_APB1ENR_USART2EN
  if (uart[uartCounter].port == USART2)
  {
    uart_handler[1] = uartCounter;
    uartIRQ     = USART2_IRQn;

    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
    RCC->AHBENR  |= RCC_AHBENR_DMA1EN;

    uart[uartCounter].txDMA = DMA1_Channel4;
    uart[uartCounter].rxDMA = DMA1_Channel5;

    uartBaudDiv = F_APB1;
  }
#endif

  uart[uartCounter].port->BRR = (uartBaudDiv/lCfg->BaudeRate) ;
  uart[uartCounter].port->CR3 = USART_CR3_DMAT | USART_CR3_DMAR  | USART_CR3_ONEBIT;

  uart[uartCounter].port->ISR;
  uart[uartCounter].port->RDR;
  uart[uartCounter].port->ICR = USART_ICR_TCCF | USART_ICR_IDLECF;

//  uart[uartCounter].port->SR = 0; // comment

  uart[uartCounter].port->CR1 = USART_CR1_TE | USART_CR1_RE | USART_CR1_IDLEIE; // | USART_CR1_TCIE;

  if (uart[uartCounter].txDMA->CCR != 0) sysErrHandler(ser_dma_cross);
  if (uart[uartCounter].rxDMA->CCR != 0) sysErrHandler(ser_dma_cross);
  uart[uartCounter].txDMA->CCR = DMA_CCR_PL_0; // mark dma
  uart[uartCounter].rxDMA->CCR = DMA_CCR_PL_0; // mark dma

  uart[uartCounter].txDMA->CPAR = (uint32_t) &uart[uartCounter].port->TDR;
  uart[uartCounter].rxDMA->CPAR = (uint32_t) &uart[uartCounter].port->RDR;

  uartResetRx(uartCounter);

  NVIC_EnableIRQ(uartIRQ);

  uart[uartCounter].port->CR1 |= USART_CR1_UE;
  return(uartCounter++);
}

void uart_irq_handler(uint8_t luart)
{
  if (uart[luart].port->ISR & USART_ISR_FE)
      uart[luart].port->ICR = USART_ICR_FECF;

  if (uart[luart].port->ISR & USART_ISR_IDLE)
  {
    uart[luart].port->ICR = USART_ICR_IDLECF;

    uartRxStop(luart);

    uartSetRxEv(luart);

    uart[luart].rxReady = 1;
  }

  if (uart[luart].port->ISR & USART_ISR_TC)
  {
    uart[luart].port->CR1 &= ~USART_CR1_TCIE;

    uart[luart].port->ICR = USART_ICR_TCCF;

    if (uart[luart].rs485port)
      pin_clr(uart[luart].rs485port, uart[luart].rs485pin);

    uartSetTxEv(luart);

    uart[luart].txBusy  = 0;
  }
}

void USART1_IRQHandler(void)
{
  uart_irq_handler(uart_handler[0]);
}

void USART2_IRQHandler(void)
{
  uart_irq_handler(uart_handler[1]);
}

