//---------------------------------------------------------------------------
// sync system
//---------------------------------------------------------------------------
#include "system.h"
//---------------------------------------------------------------------------
// sync variables
//---------------------------------------------------------------------------
uint8_t sync_1Hz_div_counter   = 10;
uint8_t sync_10Hz_div_counter  = 10;
uint8_t sync_100Hz_div_counter = 10;

uint8_t sync_1kHz_counter  = 0;
uint8_t sync_100Hz_counter = 0;
uint8_t sync_10Hz_counter  = 0;
uint8_t sync_1Hz_counter   = 0;

uint8_t sync_1kHz_num  = 0;
uint8_t sync_100Hz_num = 0;
uint8_t sync_10Hz_num  = 0;
uint8_t sync_1Hz_num   = 0;

uint8_t sync_flag = 0;
//-----------------------------------------------------------------------------
// st variables
//-----------------------------------------------------------------------------
st_task_t st_task[stCallSize];
uint8_t   stCounter = 0;
uint8_t   stNum     = 0;
//-----------------------------------------------------------------------------
// rts variables
//-----------------------------------------------------------------------------
#if rtsCallSize != 0

rts_t     rtsTasks[rtsCallSize];
uint8_t   rtsCount   = 0;
uint8_t   rtsIndex   = 0;
uint8_t*  rts_SaveSP;

#endif
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
#include "sys_drv.c"
//--------------------------------------------------------------------------
// sync Main
//--------------------------------------------------------------------------
void syncMain(void)
{
  if (sync_flag == 0) return;

  sync_flag = 0;

#if rtsCallSize != 0
  rtsTick();
#endif

  if (sync_1kHz_counter == 0) sync_1kHz_counter = sync_1kHz_num;

  // 1kHz entry
  if (--sync_100Hz_div_counter == 0)
  {
  // 100Hz entry
    sync_100Hz_div_counter = 10;

    if (sync_100Hz_counter == 0) sync_100Hz_counter = sync_100Hz_num;

    if (--sync_10Hz_div_counter == 0)
    {
  // 10Hz entry
      sync_10Hz_div_counter = 10;

      if (sync_10Hz_counter == 0) sync_10Hz_counter = sync_10Hz_num;

      if (--sync_1Hz_div_counter == 0)
      {

  // 1Hz entry
        sync_1Hz_div_counter = 10;

        if (sync_1Hz_counter == 0) sync_1Hz_counter = sync_1Hz_num;
      }
    }
  }

}
//-----------------------------------------------------------------------------
// st manager (sequential task switcher)
//-----------------------------------------------------------------------------
void stSetEvent(uint8_t taskID)
{
  st_task[taskID].flags = 1;
}
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
uint8_t stAddTask(fptr stHandler, uint8_t stTaskFlags)
{

  if (stNum  == stCallSize) sysErrHandler(ser_st_ovf);

  switch (stTaskFlags)
  {
  case stf_event:
       break;

  case stf_1kHz:
       sync_1kHz_num++;
       break;

  case stf_100Hz:
       sync_100Hz_num++;
       break;

  case stf_10Hz:
       sync_10Hz_num++;
       break;

  case stf_1Hz:
       sync_1Hz_num++;
       break;

  case stf_ring:
       break;
  }

  st_task[stNum].handler = stHandler;

  if (stTaskFlags == stf_event) st_task[stNum].flags = 0;
    else st_task[stNum].flags = stTaskFlags;

  return(stNum++);
}
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void stSleepTask(uint8_t task)
{
  if (st_task[task].flags == stf_event) return;

  switch (st_task[task].flags)
  {
  case stf_nocall:
       break;

  case stf_ring:
       st_task[task].flags = 0;
       break;

  case stf_1kHz:
       if (sync_1kHz_num > 0) sync_1kHz_num--;
       st_task[task].flags = 0;
       break;

  case stf_100Hz:
       if (sync_100Hz_num > 0) sync_100Hz_num--;
       st_task[task].flags = 0;
       break;

  case stf_10Hz:
       if (sync_10Hz_num > 0) sync_10Hz_num--;
       st_task[task].flags = 0;
       break;

  case stf_1Hz:
       if (sync_1Hz_num > 0) sync_1Hz_num--;
       st_task[task].flags = 0;
       break;
  }
}
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void stActivateTask(uint8_t task, uint8_t stTaskFlags)
{
  if (st_task[task].flags != 0) return;

  st_task[task].flags = stTaskFlags;

  if ( (stTaskFlags == stf_event)  ||
       (stTaskFlags == stf_nocall) ||
       (stTaskFlags == stf_ring)
     ) return;

  switch (st_task[task].flags)
  {
  case stf_1kHz:
       sync_1kHz_num++;
       break;

  case stf_100Hz:
       sync_100Hz_num++;
       break;

  case stf_10Hz:
       sync_10Hz_num++;
       break;

  case stf_1Hz:
       sync_1Hz_num++;
       break;
  }
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void stMain(void)
{
  if (stNum == 0) return;

  if (stCounter == stNum) stCounter = 0;

  if (st_task[stCounter].flags == 0)  {stCounter++; return;}

  switch(st_task[stCounter].flags) {

  case (stf_event):
    st_task[stCounter].flags = 0;
    st_task[stCounter].handler();
    break;

  case stf_1kHz:
    if ( sync_1kHz_counter > 0 )
    {
      st_task[stCounter].handler();
      sync_1kHz_counter--;
    }
    break;

  case stf_100Hz:
    if ( sync_100Hz_counter > 0 )
    {
      st_task[stCounter].handler();
      sync_100Hz_counter--;
    }
    break;

  case stf_10Hz:
    if ( sync_10Hz_counter > 0 )
    {
      st_task[stCounter].handler();
      sync_10Hz_counter--;
    }
    break;

  case stf_1Hz:
    if ( sync_1Hz_counter > 0 )
    {
      st_task[stCounter].handler();
      sync_1Hz_counter--;
    }
    break;

  case stf_ring:
    st_task[stCounter].handler();
    break;

  }

  stCounter++;
}
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
uint8_t stGetTaskID(void) { return stCounter;}
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void sysMain(void)
{
    syncMain();

    stMain();

#if rtsCallSize != 0
    rtsMain();
#endif
}
//-------------------------------------------------
// chip clock system: init
//-------------------------------------------------
void sysInit(void)
{
  uint8_t i;

  clkInit();
  nvicInit();
  syncInit();

  for (i=0; i<stCallSize; i++)
    {
        st_task[i].flags = 0;
        st_task[i].handler = 0;
    }

#if rtsCallSize != 0
  rtsInit();
#endif
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// RTS
#if rtsCallSize != 0
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
extern void rts_entry(void* lPtr);
extern void rts_exit(void* lPtr);
extern void rts_move_regs(fptr task, uint32_t* sstack_end);


void rtsMain(void)
{
  if (rtsCount == 0) return;

  if (rtsTasks[rtsIndex].flags == 1)
  {
    rtsTasks[rtsIndex].retcode = 0;

    rtsTasks[rtsIndex].flags = 0;

    rts_entry(&rtsTasks[rtsIndex].SP);

    if (rtsTasks[rtsIndex].flags == 0)
      rtsTasks[rtsIndex].flags = rtsTasks[rtsIndex].retcode;
  };

  if (++rtsIndex == rtsCount) rtsIndex = 0;
}

void rtsTick(void)
{
  uint8_t i;

  if (rtsCount == 0) return;

  for (i = 0; i < rtsCount; i++)

  if (rtsTasks[i].delay) rtsTasks[i].flags = (--rtsTasks[i].delay == 0);
}

void rtsDelay(uint16_t delay_l)
{
  if (delay_l == 0) return;

  rtsTasks[rtsIndex].delay = delay_l;

  rts_exit(&rtsTasks[rtsIndex].SP);
}

void rtsSkip(void)
{
  rtsTasks[rtsIndex].retcode = 1;

  rts_exit(&rtsTasks[rtsIndex].SP);
}

void rtsSleep(void)
{
  rtsTasks[rtsIndex].retcode = 0;

  rts_exit(&rtsTasks[rtsIndex].SP);
}

void rtsSetEvent(uint8_t task_index_l)
{
  if (task_index_l > (rtsCount-1) ) return;

  rtsTasks[task_index_l].flags = 1;
}

void rtsWake(uint8_t task_index_l)
{
  if (task_index_l > (rtsCount-1) ) return;

  rtsTasks[task_index_l].flags = 1;
}


uint8_t rtsAddTask(fptr task, uint32_t* stack_top, uint32_t activity)
{
  if (rtsCount == rtsCallSize) sysErrHandler(ser_rts_ovf);

  rts_move_regs(task, stack_top);
  rtsTasks[rtsCount].SP = stack_top - _RTS_SAVE_SIZE;
  rtsTasks[rtsCount].flags = activity;
  rtsTasks[rtsCount].delay = 0;
  rtsTasks[rtsCount].retcode = 0;

  return(rtsCount++);
}

void rtsWaitEvent(void)
{
  rts_exit(&rtsTasks[rtsIndex].SP);
}

void rtsInit(void)
{
}

#endif

#if rtsCallSize == 0

uint8_t*  rts_SaveSP;

void rtsWaitEvent(void)
{
}

void rtsSetEvent(uint8_t task_index_l)
{
}

#endif


//-----------------------------------------------------------------------------
