
#ifndef _HK_PINS_H
#define _HK_PINS_H

#if defined(__ICCAVR__)

#ifndef __XMEGA_CORE__
    #define pin_set(port,pin) port |= ((unsigned char) (1<<pin))
    #define pin_clr(port,pin) port &= ~ ( (unsigned char) (1<<pin) )
    #define pin_xor(port,pin) port ^= ((unsigned char) (1<<pin))

    #define pin_read(port,pin) (port & (1<<pin))
#else
    #define pin_set(port,pin) port.OUTSET = ((unsigned char) (1<<pin))
    #define pin_clr(port,pin) port.OUTCLR = ((unsigned char) (1<<pin))
    #define pin_xor(port,pin) port.OUTTGL = ((unsigned char) (1<<pin))

    #define pin_read(port,pin) (port.IN & (1<<pin))
#endif //__XMEGA_CORE__

#endif

#if defined(__arm__)
    #define pin_set(port,pin) port->ODR |=  (1<<(pin))
    #define pin_clr(port,pin) port->ODR &= ~(1<<(pin))
    #define pin_xor(port,pin) port->ODR ^=  (1<<(pin))

    #define pin_read(port,pin) ( port->IDR & (1<<(pin)) )
    #define pin_readn(port,pin) ( ~port->IDR & (1<<(pin)) )

#endif //defined(__arm__)

#endif // _HK_PINS_H


