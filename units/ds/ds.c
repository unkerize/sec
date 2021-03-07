//------------------------------------------------------------------------------------------
// DS18B20
//------------------------------------------------------------------------------------------

#include <stdlib.h>
#include <string.h>

#include "system.h"
#include "pins.h"
#include "crc.h"
#include "gpio.h"
#include "tim.h"
#include "ds.h"
#include "dscfg.h"

// [ --- config
TIM_TypeDef*   ds_tim;
uint8_t        ds_chn;

GPIO_TypeDef*  ds_port;
uint8_t        ds_pin;
uint8_t        ds_alterf;

fptr ds_pre_handler  = 0;
fptr ds_post_handler = 0;
// ] --- config

// [ - rts integration
#define  ds_rts_stack_size 128//128 //64
uint32_t ds_rts_stack[ds_rts_stack_size];
uint8_t  ds_task_ID;
// ] - rts integration

uint8_t   ds_scratch[10] __attribute__((aligned (8)));

#if DS_USE_MULTIPOINT == 1
  uint8_t ds_search_done;
#else
  #define ds_search_done 0
#endif

// 1-wire driver
#include "ds_1w.c"


// keys cycle
#if DS_CFG_TYPE == 0x0
  #include "ds_temp.c"
#endif

// temp cycle
#if DS_CFG_TYPE == 0x1
  #include "ds_keys.c"
#endif

fptr ds_init(ds_cfg_td* cfg)
{
  ds_tim  = cfg->tim;
  ds_chn  = cfg->chn;
  ds_port = cfg->port;
  ds_pin  = cfg->pin;
  ds_alterf  = cfg->alterf;

  ds_pre_handler = cfg->pre_handler;
  ds_post_handler = cfg->post_handler;

        if (ds_chn == 0)
        {
    ds_req_pulse_reg = (__IO uint16_t*) &ds_tim->CCR1;
    ds_ack_pulse_reg = (__IO uint16_t*) &ds_tim->CCR2;
    ds_timing_reg    = (__IO uint16_t*) &ds_tim->CCR3;;
        }

        if (ds_chn == 1)
        {
    ds_req_pulse_reg = (__IO uint16_t*) &ds_tim->CCR2;
    ds_ack_pulse_reg = (__IO uint16_t*) &ds_tim->CCR1;
    ds_timing_reg    = (__IO uint16_t*) &ds_tim->CCR3;;
        }

        if (ds_chn == 2)
        {
    ds_req_pulse_reg = (__IO uint16_t*) &ds_tim->CCR3;
    ds_ack_pulse_reg = (__IO uint16_t*) &ds_tim->CCR4;
    ds_timing_reg    = (__IO uint16_t*)&ds_tim->CCR1;;
        }

        if (ds_chn == 3)
        {
    ds_req_pulse_reg = (__IO uint16_t*) &ds_tim->CCR4;
    ds_ack_pulse_reg = (__IO uint16_t*) &ds_tim->CCR3;
    ds_timing_reg    = (__IO uint16_t*) &ds_tim->CCR1;;
        }

  ds_low_level_init();

  ds_task_ID = rtsAddTask(&ds_manager, &ds_rts_stack[ds_rts_stack_size-1], 1);

  return &ds_irq_handler;
}

uint8_t ds_get_type(void)
{
  return DS_CFG_TYPE;
}

uint8_t ds_get_multi(void)
{
  return DS_USE_MULTIPOINT;
}

int8_t ds_count(void)
{
  return ds_dev_count;
}

uint64_t* ds_read_key(void)
{
  return (uint64_t*) ds_scratch;
}

uint8_t ds_get_search_status(void)
{
  return ds_search_done;
}
