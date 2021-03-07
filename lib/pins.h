
#ifndef _HK_PINS_H
#define _HK_PINS_H

//#if defined(STM32F303x6) || defined(STM32F303x8) || \
//    defined(STM32F303xB) || defined(STM32F303xC)

//    #define pin_set(port,pin) port->BSRRL = (1<<(pin))
//    #define pin_clr(port,pin) port->BSRRH = (1<<(pin))

//#else
    #define pin_set(port,pin) port->BSRR = (1<<(pin))
    #define pin_clr(port,pin) port->BSRR = (1<<(pin+16))
//#endif

    #define pin_xor(port,pin) if (port->ODR & (1<<pin) ) pin_clr(port,pin); else pin_set(port,pin)

    #define pin_read(port,pin) ( port->IDR & (1<<(pin)) )
    #define pin_readn(port,pin) ( ~port->IDR & (1<<(pin)) )

#endif // _HK_PINS_H


