//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
#include "system.h"
#include "mface.h"
#include "decseg.h"
#include "vis_mf.h"
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
static mf_t*   vis_mf;
static uint8_t vis_blink_index = 0xff;
static uint8_t vis_led_index = 0;
static uint8_t vis_segs[MF_SEG_NUM];
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void vis_out(uint16_t value, uint8_t pos, uint8_t dot_pos)
{
   if (dot_pos > 4) return;

   decseg_out_i16(value, &vis_mf->segs[pos], 4);

   if (dot_pos == 0) return;

   vis_mf->segs[pos+dot_pos-1] |= segDP;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void vis_handler(void)
{
/*
        uint8_t i;

        for (i = 0; i < MF_SEG_NUM; i++)
        {
    vis_mf->segs_blink[i] = 0;
    vis_mf->segs[i] = dec_to_seg(vis_segs[i]);
        }

        if (vis_blink_index != 0xff) vis_mf->segs_blink[vis_blink_index] = 1;

        for (i = 0; i < MF_LED_NUM; i++) vis_mf->leds[i] = 0;

        vis_mf->leds[vis_led_index] = 2;
*/
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void kh1(uint32_t edge) //up
{
        if (edge == 0) return;

        if (vis_blink_index == 0xff) return;

  if (vis_segs[vis_blink_index] < 9)  vis_segs[vis_blink_index]++;
}

void kh2(uint32_t edge) //dn
{
        if (edge == 0) return;

        if (vis_blink_index == 0xff) return;

        if (vis_segs[vis_blink_index] > 0)  vis_segs[vis_blink_index]--;
}

void kh3(uint32_t edge)  //left
{
        if (edge == 0) return;

        if (vis_blink_index == 0xff) return;

        if (vis_blink_index > 0 ) vis_blink_index--;
}

void kh4(uint32_t edge)  //right
{
        if (edge == 0) return;

        if (vis_blink_index == 0xff) return;

        if (vis_blink_index < (MF_SEG_NUM-1) ) vis_blink_index++;
}

void kh5(uint32_t edge) // esc
{
        if (edge == 0) return;

        vis_blink_index = 0xff;
}

void kh6(uint32_t edge) // enter
{
        if (edge == 0) return;

        if (vis_blink_index == 0xff) {vis_blink_index = 0; return;}
          else {vis_blink_index = 0xff; return;}
}

void kh7(uint32_t edge)
{
        if (edge == 0) return;

        if (++vis_led_index == MF_LED_NUM) vis_led_index = 0;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
static const kb_cfg_t vis_kb_cfg[MF_KEY_NUM] = {
  {&kh1, 1},  //up
  {&kh2, 1},  //dn
  {&kh3, 1},     //left
  {&kh4, 1},     //right
  {&kh5, 0},     //esc
  {&kh6, 0},     //enter
  {&kh7, 0} };   //res

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void vis_init(void)
{
        vis_mf = mf_init( (kb_cfg_t*) vis_kb_cfg);
        stAddTask(&vis_handler,stf_100Hz);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
