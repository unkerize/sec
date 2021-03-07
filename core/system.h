//----------------------------------------------
//----------------------------------------------
#ifndef _HK_SYSTEM_H
#define _HK_SYSTEM_H
//----------------------------------------------
//----------------------------------------------
// select chip harware abstraction file
#include <stdint.h>
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// system constants
//-----------------------------------------------------------------------
#define NULL 0

// st const
#define stf_nocall  0
#define stf_event   1
#define stf_ring    2

#define stf_100Hz 3
#define stf_10Hz  4
#define stf_1Hz   5
#define stf_1kHz  6
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
#define ser_no_err    0
#define ser_st_ovf    1
#define ser_rts_ovf   2
#define ser_dma_cross 3
#define ser_heap_err  4

#define ser_adc_cross  5
#define ser_adc_expnum 6
#define ser_adc_param  7

#define ser_par_err   8
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------

// function pointer type
typedef void (*fptr)(void)   ;  // pointer to task
// function pointer type
typedef void (*fpptr)(uint32_t)   ;  // pointer to task
// st list structure
typedef struct {
  uint8_t flags;
  fptr handler;         // pointer to task
} st_task_t;
//-----------------------------------------------------------------------
//-------------------------------------------------------------------------
#define _RTS_SAVE_SIZE 11 // R3..R12, PSR, RET

typedef struct {
  uint32_t flags;
  uint32_t retcode;
  uint32_t* SP;
  uint32_t delay;
} rts_t ;
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
#include "config.h"
#include "chipsel.h"
#include "sys_drv.h"
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
#if rtsCallSize != 0
void rtsMain(void);
void rtsInit(void);
void rtsSetEvent(uint8_t task);
void rtsWaitEvent(void);
void rtsTick(void);
void rtsDelay(uint16_t msDelay);
void rtsSkip(void);

void rtsSleep(void);
void rtsWake(uint8_t task_index_l);


uint8_t rtsAddTask(fptr task, uint32_t* stack_top, uint32_t activity);

#endif
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// st
uint8_t stAddTask(fptr stHandler, uint8_t stTaskFlags);
void    stSetEvent(uint8_t taskID);
uint8_t stGetTaskID(void);

void stSleepTask(uint8_t task);
void stActivateTask(uint8_t task, uint8_t stTaskFlags);

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// system
void sysInit(void);
void sysMain(void);
void syncMain(void);
void stMain(void);
void rtsMain(void);

extern void sysErrHandler(uint32_t err);

void usecDelay(uint32_t usDel);
void msecDelay(uint16_t msDel);

#endif  // _HK_SYSTEM_H
