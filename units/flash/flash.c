//------------------------------------------------------------------------------------------
// FLASH
//------------------------------------------------------------------------------------------
#include "system.h"
#include "flash.h"
#include "crc.h"
#include <string.h>

volatile uint16_t* flashAddr ;
uint16_t* flashDataAddr ;
uint16_t  flashDataSize;
uint16_t  flashCounter;
uint8_t   flash_BUSY;
uint8_t   flash_CHECK;

uint8_t flashWrite(void)
{
  uint16_t  crc;

  if (flash_BUSY)  return 1;

  flash_BUSY = 1;

  flashCounter = 0;

  if (flash_CHECK == 2)
  {
    crc = calcCRC16((void*) flashDataAddr, flashDataSize*2 - 2);
    flashDataAddr[flashDataSize*2 - 1] = crc;
  }

  FLASH->CR &= ~(FLASH_CR_PG | FLASH_CR_STRT);
  FLASH->CR |= FLASH_CR_PER;
  FLASH->AR = flash_DATA_START;

  FLASH->CR |= FLASH_CR_EOPIE;
  FLASH->CR |= FLASH_CR_STRT;

  return 0;
}

uint8_t flashInit(void* data, void* data_default, uint16_t size, uint16_t options)
{
  uint8_t opt_check;
  uint8_t opt_rewrite;

  opt_check   =  options & 0x3;
  flash_CHECK = opt_check;

  opt_rewrite = (options>>2) & 0x3;

  if (size > flash_PAGE_SIZE) return 0xff;

  if (data == 0) return 0xff;

  if ( (data_default == 0) && (opt_check != 0) )  return 0xff;

  FLASH->KEYR = FLASH_FKEY1;
  FLASH->KEYR = FLASH_FKEY2;
  NVIC_EnableIRQ(FLASH_IRQn);

  flash_BUSY = 0;

  flashAddr = (uint16_t*) flash_DATA_START;
  flashDataAddr  = (uint16_t*) data;
  flashDataSize  = size/2;

  switch (opt_check) {
  case 0: // no check
  {
    memcpy(data,(void*) flashAddr, flashDataSize*2);
    return 0;
  }

  case 1: // check 0xffff
  {
    if (flashAddr[0] == 0xffff)
    {
      memcpy(data, data_default, flashDataSize*2);

      if (opt_rewrite == 0) return 1;

      flashWrite();

      return 2;
    } else
    {
      memcpy(data,(void*) flashAddr, flashDataSize*2);
      return 0;
    }
  }

  case 2: // check crc16
  {
    memcpy(data,(void*) flashAddr, flashDataSize*2);

    if (calcCRC16(data, flashDataSize*2) != 0)
    {
      memcpy(data, data_default, flashDataSize*2);

      if (opt_rewrite == 0) return 1;

      flashWrite();

      return 2;
    } else
    {
      return 0;
    }
  }
  }
  
  return 0;
}

void FLASH_IRQHandler(void)
{
  FLASH->SR |= FLASH_SR_EOP;

  FLASH->CR &= ~(FLASH_CR_PER | FLASH_CR_STRT);
  FLASH->CR |= FLASH_CR_PG;

  if (flashCounter == flashDataSize)
  {
    flash_BUSY = 0;
    FLASH->CR &= ~(FLASH_CR_EOPIE | FLASH_CR_PG) ;
    return;
  } else
  {
//    (uint16_t) flashAddr[flashCounter] = (uint16_t) flashDataAddr[flashCounter];
    flashAddr[flashCounter] = flashDataAddr[flashCounter];
    flashCounter++;
  }
}

uint8_t flashBusy(void)
{
  return flash_BUSY;
}
