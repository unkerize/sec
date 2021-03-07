
#include "system.h"
#include "pins.h"
#include "term.h"
#include <stdio.h>
#include <string.h>
#include "console.h"

static uint8_t con_rtsh;
static uint8_t con_kb_pressed = 0;
static uint8_t con_kb_code;
static uint8_t con_str_cnt    = 0;
static uint8_t con_str_enter  = 0;

static char con_str_buf[CON_STR_MAX_LEN];
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// terminal
static void con_Main(void)
{
  while(1)
  {
    if ( fkbpress() )
    {
      con_kb_pressed = 1;

      con_kb_code = fgetc(&__stdin);

      if (!con_str_enter)
      {
        if (con_str_cnt < CON_STR_MAX_LEN) con_str_buf[con_str_cnt++] = con_kb_code;

        if (con_str_buf[con_str_cnt - 1] == 13)
        {
          con_str_buf[con_str_cnt - 1] = 0;

          con_str_enter = 1;
        }
      }

      pin_xor(ledPORT, kbLED);
    }

    rtsDelay(20);
  }
}
//--------------------------------------------------------------------------
void con_read_str(char* str)
{
  con_str_cnt = 0;
  con_str_enter = 0;
  con_kb_pressed = 0;

  while (!con_str_enter) rtsDelay(20);

  strncpy(str, con_str_buf, CON_STR_MAX_LEN);

  con_str_enter = 0;
  con_kb_pressed = 0;
}
//--------------------------------------------------------------------------
uint8_t con_kbpressed(void)
{
  uint8_t temp;

  temp = con_kb_pressed;

  con_kb_pressed = 0;

return (temp == 1);
}
//--------------------------------------------------------------------------
uint8_t con_kbcode(void)
{
  return con_kb_code;
}
//--------------------------------------------------------------------------
void con_Init(void)
{
  con_rtsh = rtsAddTask(&con_Main, 1);

  terminalInit(con_rtsh);
}
//--------------------------------------------------------------------------
