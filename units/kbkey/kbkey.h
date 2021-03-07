#ifndef _HK_KBKEY_H
#define _HK_KBKEY_H
//-------------------------------------------------------
//-------------------------------------------------------
#include "system.h"
//-------------------------------------------------------
//-------------------------------------------------------

#define KB_UNPRESS 0         // dont change !
#define KB_PRESS   1         // dont change !
#define KB_CLICK_PRESS  2
#define KB_LONG_PRESS   3
#define KB_LONG_UNPRESS 4


#define KB_CLH_NON    0
#define KB_CLH_CLICK  1
#define KB_CLH_HOLD   2

#pragma pack(push,1)

typedef struct {
  volatile uint32_t* port;
  uint16_t           pin_mask;
  uint8_t            flags;  // bit0 = inverse
} din_ports_t;

typedef struct {
  fpptr handler;
  uint8_t click;
} kb_cfg_t;

typedef struct {
  uint8_t counter;
  uint8_t prev;
  uint8_t state;
} din_t;

typedef struct {
  uint8_t prev;
  uint8_t click_counter;
  uint8_t hold_counter;
  uint8_t hold;
} kb_t;

typedef struct{
  din_ports_t* din_port;
  uint8_t      din_size;
  uint8_t      din_const;
  uint8_t      din_os_flag;

  kb_cfg_t*    kb_cfg;
  uint8_t      kb_size;
  uint8_t      kb_click_const;
  uint8_t      kb_hold_const;
  uint8_t      kb_os_flag;
} kbp_t;

#pragma pack(pop)
//-------------------------------------------------------
//-------------------------------------------------------
din_t* kb_init(kbp_t* l_kbp);
//-------------------------------------------------------
//-------------------------------------------------------
#endif //_HK_KBKEY_H
