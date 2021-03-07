
#ifndef __RF24_CONFIG_H__
#define __RF24_CONFIG_H__

#include "system.h" 

  /*** USER DEFINES:  ***/
  //#define FAILURE_HANDLING
  //#define SERIAL_DEBUG
  //#define SPI_UART  // Requires library from https://github.com/TMRh20/Sketches/tree/master/SPI_UART
  //#define SOFTSPI   // Requires library from https://github.com/greiman/DigitalIO

  /**********************/
  #define rf24_max(a,b) (a>b?a:b)
  #define rf24_min(a,b) (a<b?a:b)

// extern HardwareSPI SPI;

 #define _BV(x) (1<<(x))

  typedef uint16_t prog_uint16_t;
  #define PSTR(x) (x)
  #define printf_P printf
  #define strlen_P strlen
  #define PROGMEM
  #define pgm_read_word(p) (*(p))

  #define PRIPSTR "%s"

#endif // __RF24_CONFIG_H__

