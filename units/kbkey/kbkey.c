//-------------------------------------------------------
//-------------------------------------------------------
#include "system.h"
#include "kbkey.h"
#include <stdlib.h>

static kbp_t* kbp;
static din_t* din;
static kb_t* kb;

void din_main(void)
{
  uint8_t i;
  uint8_t rstate;
  uint8_t prev;

  for (i = 0; i < kbp->din_size; i++)
  {
    rstate = ( *(kbp->din_port[i].port) & kbp->din_port[i].pin_mask) != 0;
    if ( (kbp->din_port[i].flags & 1) == 1) rstate = 1 - rstate;

    prev = din[i].prev;
    din[i].prev = rstate;

    if (rstate != prev) {din[i].counter = 0; continue;}

    if (din[i].counter != kbp->din_const) {din[i].counter++; continue;}

    din[i].state = rstate;
  }
}


void kb_main(void)
{
  uint8_t i;
  uint8_t prev;

  for (i = 0; i < kbp->kb_size; i++)
  {
    if (kbp->kb_cfg[i].handler == 0) continue;

    prev = kb[i].prev;

    kb[i].prev = din[i].state;

    if (din[i].state != prev)
    {
      if ( ( kbp->kb_cfg[i].click == KB_CLH_HOLD) &&
           (din[i].state == 0) &&
           (kb[i].hold_counter == kbp->kb_hold_const) )
        kbp->kb_cfg[i].handler(KB_LONG_UNPRESS);
      else
        kbp->kb_cfg[i].handler(din[i].state);

      kb[i].click_counter = 0;
      kb[i].hold_counter  = 0;

      continue;
    }
    else
    {
      if ( (kbp->kb_cfg[i].click == 0) || (din[i].state == 0) ) continue;

      if (kb[i].hold_counter != kbp->kb_hold_const)
      {
        kb[i].hold_counter++;

        if ((kb[i].hold_counter == kbp->kb_hold_const) && (kbp->kb_cfg[i].click == KB_CLH_HOLD))
          kbp->kb_cfg[i].handler(KB_LONG_PRESS);
        continue;
      }

      if (kbp->kb_cfg[i].click != KB_CLH_CLICK) return;

      if (kb[i].click_counter != kbp->kb_click_const) {kb[i].click_counter++; continue;}
      else {kb[i].click_counter = 0; kbp->kb_cfg[i].handler(KB_CLICK_PRESS) ;}
    }
  }
}

din_t* kb_init(kbp_t* l_kbp)
{
  uint8_t i;

  kbp = l_kbp;

  if (kbp->din_port == 0) return 0;

  din = malloc(sizeof(din_t)*kbp->din_size);

  if (din == 0) sysErrHandler(ser_heap_err);

  for (i = 0; i < kbp->din_size; i++)
  {
    din[i].counter = 0;
    din[i].prev = 0;
    din[i].state = 0;
  }

  stAddTask(&din_main, kbp->din_os_flag);

  if (kbp->kb_cfg == 0) return din;

  kb = malloc(sizeof(kb_t)*kbp->kb_size);

  if (kb == 0) sysErrHandler(ser_heap_err);

  for (i = 0; i < kbp->kb_size; i++)
  {
    kb[i].prev = 0;
    kb[i].click_counter = 0;
    kb[i].hold_counter  = 0;
    kb[i].hold          = 0;

  }

  stAddTask(&kb_main, kbp->kb_os_flag);

  return din;
}
