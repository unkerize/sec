
#ifndef _HK_DECSEG_H
#define _HK_DECSEG_H

#include <stdint.h>
#include "segfont.h"

void ui16_to_bcd(uint16_t value, uint8_t* buffer, uint8_t decnum);
uint8_t dec_to_seg(uint8_t dec);
void decbuf_to_seg(uint8_t* decbuf, uint8_t decnum);
void decseg_out_i16(uint16_t value, uint8_t* buffer, uint8_t decnum);


#endif //_HK_DECSEG_H
