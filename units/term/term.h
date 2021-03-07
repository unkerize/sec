
#ifndef _HK_TERMINAL_H
#define _HK_TERMINAL_H

#include "system.h"
#include "gpio.h"

#define terminalMaxLen     160
#define terminalPORT       USART2
#define terminalBaudeRate  115200

#define terminal485port 0 //gpioPA  // 0-if not used (gpioNON)
#define terminal485pin  11


void terminalInit(uint8_t lrtsh);
uint8_t fkbpress(void);


#endif //_HK_TERMINAL_H

