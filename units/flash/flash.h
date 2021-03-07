
#ifndef _HK_FLASH_H
#define _HK_FLASH_H


#ifndef FLASH_FKEY1
 #define FLASH_FKEY1 0x45670123
 #define FLASH_FKEY2 0xCDEF89AB
#endif

// stm32f103RB
// 128k, 1k page
#define flash_DATA_OFS    (flash_SIZE-flash_PAGE_SIZE)
#define flash_DATA_START (0x08000000+flash_DATA_OFS)


uint8_t flashInit(void* data, void* data_default, uint16_t size, uint16_t options);
uint8_t flashWrite(void);
uint8_t flashBusy(void);


#endif
