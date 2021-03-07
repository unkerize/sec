

#ifndef _HK_MFACE_H
#define _HK_MFACE_H
//-------------------------------------------------------
//-------------------------------------------------------

#include "kbkey.h"

#define MF_SPI_SIZE 5
#define MF_COL_NUM 4
#define MF_RAW_NUM 4
#define MF_LED_NUM 4
#define MF_KEY_NUM 7

#define MF_SEG_NUM (MF_COL_NUM*MF_RAW_NUM)

#define MF_COL_1  (1<<5)
#define MF_COL_2  (1<<7)
#define MF_COL_3  (1<<4)
#define MF_COL_4  (1<<6)

#define MF_LED_1  (1<<0)
#define MF_LED_2  (1<<1)
#define MF_LED_3  (1<<2)
#define MF_LED_4  (1<<3)

#define MF_KEY_1  (1<<4)
#define MF_KEY_2  (1<<5)
#define MF_KEY_3  (1<<3)
#define MF_KEY_4  (1<<6)
#define MF_KEY_5  (1<<2)
#define MF_KEY_6  (1<<0)
#define MF_KEY_7  (1<<1)


#pragma pack(push,1)
typedef struct{
  uint8_t segs[MF_COL_NUM*MF_RAW_NUM];
  uint8_t segs_blink[MF_COL_NUM*MF_RAW_NUM];
  uint8_t leds[MF_LED_NUM];
  uint8_t keys[MF_KEY_NUM];
} mf_t;
#pragma pack(pop)
//-------------------------------------------------------
//-------------------------------------------------------
mf_t* mf_init(kb_cfg_t* l_keys_cfg);
//-------------------------------------------------------
//-------------------------------------------------------
#endif //_HK_MFACE_H
