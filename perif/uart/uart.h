
#ifndef _HK_UART_H
#define _HK_UART_H

#include <stdint.h>



#define uartNumber 2

#define uartOsNON  (0)
#define uartOsST   (1)
#define uartOsRTS  (2)


typedef struct {
    USART_TypeDef* Port;
    uint32_t       BaudeRate;

    uint8_t*       rxBuf;
    uint16_t       rxBufSize;

    GPIO_TypeDef*  Port485;
    uint8_t        Pin485;

    uint8_t        osTxFlag;
    uint8_t        osTxIndex;
    uint8_t        osRxFlag;
    uint8_t        osRxIndex;
} uartCfgType;

uint8_t uartTxBusy(uint8_t luart);
uint8_t uartRxReady(uint8_t luart);

void uartResetRx(uint8_t luart);
uint16_t uartGetRxSize(uint8_t luart);


void uartTxBlock(uint8_t luart, uint8_t* ltxBuf, uint16_t ltxSize);
void uartSetBaudeRate(uint8_t luart, uint32_t lBaudeRate);

uint8_t uartAdd(uartCfgType* lCfg);

#endif //_HK_UART_H
