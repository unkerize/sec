
#ifndef _HK_CRC_H
#define _HK_CRC_H

#include <stdint.h>

uint8_t calcCRC8(uint8_t *lBuf, uint8_t lSize);
uint16_t calcCRC16(uint8_t* buf, uint8_t size);
uint8_t testForZero(uint8_t* buf, uint16_t size);
uint8_t checkCRC8(uint8_t *lBuf, uint16_t lSize);

#endif //_HK_CRC_H



