
#ifndef _HK_NTS_H
#define _HK_NTS_H

#include "system.h"
#include "uart.h"
#include "crc.h"

//----------------------------------------------------------------
//----------------------------------------------------------------
#define ntsNumber   2
#define ntsCmNumber 20
//----------------------------------------------------------------
//----------------------------------------------------------------
#define ntsCRC_NON 0  // no crc gen/test
#define ntsCRC8    1  // dallas microlan
#define ntsCRC16   2  // modbus
//----------------------------------------------------------------
//----------------------------------------------------------------
typedef struct {
    uint8_t uarth;
    uint8_t rxsth;
    uint8_t txsth;

    USART_TypeDef*  port;
    uint32_t        BaudeRate;
    GPIO_TypeDef*   dirPort;
    uint8_t         dirPin;

    uint8_t  Addr;
    uint8_t  crcType;
    uint8_t  Ack;
    uint8_t  sepTx;

    uint16_t txSize;
    uint8_t* txBuffer;

    uint16_t rxSize;
    uint8_t* rxBuffer;
    uint16_t rxBufSize;

    fptr     cmList[ntsCmNumber];

} ntsType;

void ntsAddCm(ntsType* ntsh, uint8_t cm_index, fptr cm_handler);
void ntsSendPack(ntsType*  ntsh);
uint8_t ntsAdd(ntsType* ntshl);


#endif //_HK_NTS_H
