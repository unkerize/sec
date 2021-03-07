
#include <stdint.h>

#include "..\..\core\system.h"
#include "..\..\perif\spi\spi.h"
#include "..\..\libs\pins.h"
#include "dsx.h"
#include "dsxcfg.h"


#ifdef  dsxfDISP_BLINK
#if     (dsxfDISP_BLINK == 0) || (dsxfDISP_BLINK > 32767)
  #error "dsxf: Error! dsxfDISP_BLINK not in range"
#endif
#endif

#ifdef  dsxfLED_BLINK
#if     (dsxfLED_BLINK == 0) || (dsxfLED_BLINK > 32767)
  #error "dsxf: Error! dsxfLED_BLINK not in range"
#endif
#endif

#define dsxfSPI_SIZE 4

dsxf_t  dsxf_l;

uint8_t dsxf_counter;
fptr    dsxf_visHandler;
uint8_t dsxf_txBuffer[dsxfSPI_SIZE];
uint8_t dsxf_rxBuffer[dsxfSPI_SIZE];

#if dsxfCOM == 0
const uint8_t dsxf_switch[3] = {(1<<6),(1<<5),(1<<7)};
#else
const uint8_t dsxf_switch[3] = {0xA0, 0xC0, 0x60};
#endif

#ifdef  dsxfDISP_BLINK
    uint16_t dsxf_disp_BlinkCounter;
#endif

#ifdef  dsxfLED_BLINK
    uint16_t dsxf_led_BlinkCounter;
#endif

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void dsxfMain(void)
{
  uint8_t del;
        uint8_t led_mask;
        uint8_t i;

#if     defined(dsxfLED_BLINK) || defined(dsxfDISP_BLINK)
        uint8_t blink;
#endif
//-----------------------------------------------------------
// latch HC595/HC165
//-----------------------------------------------------------
  pin_set(dsxfLTCport, dsxfLTCpin);
  for (del = dsxfLTCdelay; del != 0; del--) {};
  pin_clr(dsxfLTCport, dsxfLTCpin);

        pin_set(ledPORT, led3);


        if (dsxf_visHandler) dsxf_visHandler();
//-----------------------------------------------------------
// process vertical 4 leds
//-----------------------------------------------------------
        led_mask = 0;
        i = 3;

#ifdef  dsxfLED_BLINK
        blink = dsxf_led_BlinkCounter <= dsxfLED_BLINK/2;

        for (i=0; i<4; i++)
                if (dsxf_l.led4Buf[i] == 2) led_mask |= blink << (3-i);
                else    led_mask |= dsxf_l.led4Buf[i] << (3-i);
#else
        for (i=0; i<4; i++)
          led_mask |= dsxf_l.led4Buf[i] << (3-i);
#endif
        led_mask = (led_mask ^ 0xf) << 1;
//-----------------------------------------------------------
// process disp
//-----------------------------------------------------------
#ifdef  dsxfDISP_BLINK
  blink = dsxf_disp_BlinkCounter <= dsxfDISP_BLINK/2;

        if (dsxf_l.dispBlink[0+dsxf_counter] && blink) dsxf_txBuffer[2] = 0;
        else dsxf_txBuffer[2] = dsxf_l.dispBuf[0+dsxf_counter];

        if (dsxf_l.dispBlink[3+dsxf_counter] && blink) dsxf_txBuffer[0] = 0;
        else dsxf_txBuffer[0] = dsxf_l.dispBuf[3+dsxf_counter];
#else
        dsxf_txBuffer[2] = dsxf_l.dispBuf[0+dsxf_counter];
        dsxf_txBuffer[0] = dsxf_l.dispBuf[3+dsxf_counter];
#endif
//-----------------------------------------------------------
// combine strobe switches & vertical 4 leds
//-----------------------------------------------------------
  dsxf_txBuffer[3] = led_mask | dsxf_switch[dsxf_counter];
//-----------------------------------------------------------
// process 8 horizontal leds
//-----------------------------------------------------------
        led_mask = 0;

#ifdef  dsxfLED_BLINK
        blink = dsxf_led_BlinkCounter <= dsxfLED_BLINK/2;

  for (i=0; i<8; i++)
          if (dsxf_l.led8Buf[i] == 2) led_mask |= blink << i;
                else led_mask |= dsxf_l.led8Buf[i] << i;
#else
  for (i=0; i<8; i++) led_mask |= dsxf_l.led8Buf[i] << i;
#endif
  dsxf_txBuffer[1] = ~( ((led_mask&7)<<1) | ((led_mask>>3)&1) | (led_mask & 0xf0) );

        if (++dsxf_counter == 3)  dsxf_counter = 0;

        dsxf_l.keys = ( (dsxf_rxBuffer[0] & 7) | ( (dsxf_rxBuffer[0] & 0xe0) >> 2) );

        spiSendPack(dsxf_txBuffer, dsxf_rxBuffer , dsxfSPI_SIZE);
				
#ifdef  dsxfLED_BLINK
    if (!--dsxf_led_BlinkCounter) dsxf_led_BlinkCounter = dsxfLED_BLINK;
#endif		

#ifdef  dsxfDISP_BLINK
    if (!--dsxf_disp_BlinkCounter) dsxf_disp_BlinkCounter = dsxfDISP_BLINK;
#endif		

  pin_clr(ledPORT, led3);

}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
dsxf_t* dsxfInit(fptr l_VisHandler)
{
  pin_clr(dsxfLTCport, dsxfLTCpin);

  spiInit();

        dsxf_counter = 0;
        dsxf_visHandler = l_VisHandler;

#ifdef  dsxfDISP_BLINK
    dsxf_disp_BlinkCounter = dsxfDISP_BLINK;
#endif

#ifdef  dsxfLED_BLINK
    dsxf_led_BlinkCounter = dsxfLED_BLINK;
#endif

#if dsxfFREQ == 1000
  stAddTask(&dsxfMain,stf_1kHz);
#else
  stAddTask(&dsxfMain,stf_100Hz);
#endif

        return &dsxf_l;
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
