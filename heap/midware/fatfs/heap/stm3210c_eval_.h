/**
  ******************************************************************************
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM3210C_EVAL_H
#define __STM3210C_EVAL_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

#include "system.h"
#include <stdint.h>
#include "hal.h"
#include "pins.h"


/*##################### SD ###################################*/
/* Chip Select macro definition */


void fat_add_rts(uint8_t rtsh);


#ifdef __cplusplus
}
#endif

#endif /* __STM3210C_EVAL_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
