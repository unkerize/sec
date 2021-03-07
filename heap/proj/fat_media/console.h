
#ifndef _HK_CONSOLE_H
#define _HK_CONSOLE_H

#include "system.h"

#define CON_STR_MAX_LEN 80

void con_read_str(char* str);
uint8_t con_kbpressed(void);
uint8_t con_kbcode(void);
void con_Init(void);


#endif //_HK_CONSOLE_H

