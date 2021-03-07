
#include <stdint.h>
#include "segfont.h"
//---------------------------------------------------------------------------
const uint8_t segFont[16] = {sym_0, sym_1, sym_2, sym_3, \
                             sym_4, sym_5, sym_6, sym_7, \
                             sym_8, sym_9, syme_A, syme_B, \
                             syme_C, syme_D, syme_E, syme_F};

const uint16_t decseg_weight_list[4] = {10000,1000,100,10};
//---------------------------------------------------------------------------
void ui16_to_bcd(uint16_t value, uint8_t* buffer, uint8_t decnum)
{

        uint16_t dec_weight;
        uint8_t  dec_index;
        uint8_t  dec_value;

        if (decnum < 2) return;

  if(value == 0)
   {
      for (dec_index = 0; dec_index < decnum; dec_index++)
                  buffer[dec_index] = 0;
   } else
         {
     dec_index = 0;

                 do
                 {
        dec_weight = decseg_weight_list[dec_index + (5-decnum)];

                          for (dec_value = 0; value >= dec_weight; dec_value++)
                                        value -= dec_weight;

                                buffer[dec_index] = dec_value;

                        }       while(++dec_index < (decnum-1) );

                        buffer[dec_index] = value;
   }

         for (dec_index = 0; (buffer[dec_index] == 0) && (dec_index < (decnum-1)) ; dec_index++)
            buffer[dec_index] = 0xff;
}
//---------------------------------------------------------------------------
uint8_t dec_to_seg(uint8_t dec)
{
  if (dec > 15) return 0;

        return segFont[dec];
}
//---------------------------------------------------------------------------
void decbuf_to_seg(uint8_t* decbuf, uint8_t decnum)
{
        while (decnum-- > 0) *decbuf++ = (*decbuf>15)?0:segFont[*decbuf];
}
//---------------------------------------------------------------------------
void decseg_out_i16(uint16_t value, uint8_t* buffer, uint8_t decnum)
{
  uint16_t dec_weight;
  uint8_t  dec_index;
  uint8_t  dec_value;

  if(value == 0)
   {
      for (dec_index = 0; dec_index < (decnum-1); dec_index++) buffer[dec_index] = 0;

      buffer[dec_index] = segFont[0];
      return;
   }

  if (decnum < 2)
  {
    buffer[0] = segFont[value & 0xf];
    return;
  }

  dec_index = 0;

  do
  {
    dec_weight = decseg_weight_list[dec_index + (5-decnum)];

    for (dec_value = 0; value >= dec_weight; dec_value++) value -= dec_weight;

    buffer[dec_index] = segFont[dec_value];

  } while(++dec_index < (decnum-1) );

  buffer[dec_index] = segFont[value];

  for (dec_index = 0; (buffer[dec_index] == segFont[0]) && (dec_index < (decnum-1)) ; dec_index++)
    buffer[dec_index] = 0;
}
