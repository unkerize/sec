
#ifndef _HK_DSX_H
#define _HK_DSX_H

#include "dsxcfg.h"

typedef struct {
    uint8_t led8Buf[8];
    uint8_t led4Buf[4];
    uint8_t dispBuf[6];
          uint8_t keys;
#ifdef  dsxfDISP_BLINK
    uint8_t dispBlink[6];
#endif
} dsxf_t;

dsxf_t* dsxfInit(fptr l_VisHandler);

#endif //_HK_DSX_H
