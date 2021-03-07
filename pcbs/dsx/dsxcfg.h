
#ifndef _HK_DSXCFG_H
#define _HK_DSXCFG_H
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
#define dsxfFREQ 1000   // 100, 1000 Hz
#define dsxfCOM  0      // 0=common cathod, 1=common anode
#define dsxfLED_BLINK  200 // if defined,  use blink, value=divfactor(1..32767)
#define dsxfDISP_BLINK 400 // if defined,  use blink, value=divfactor(1..32767)

#define dsxfLTCport   GPIOA
#define dsxfLTCpin    4
#define dsxfLTCdelay  1
//-----------------------------------------------------------------------------
#endif //_HK_DSXCFG_H

