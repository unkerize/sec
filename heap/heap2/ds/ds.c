//------------------------------------------------------------------------------------------
// DS18B20
//------------------------------------------------------------------------------------------
#include "ds.h"
#include "gpio.h"
#include "crc.h"

uint16_t ds_ack_pulse;
uint8_t  ds_task_ID;

uint8_t ds_present;
uint8_t ds_scratch[9];
int16_t ds_temp;
uint8_t ds_error = 0;
fptr    ds_ready_handler = 0;


void DS_TIM_IRQ_HANDLER(void)
{
  DS_TIM->CR1 &= ~TIM_CR1_CEN;

  DS_TIM->SR &= ~DS_SR_MASK;

  rtsSetEvent(ds_task_ID);
}

void ds_low_level_init(void)
{
  gpioCFG(DS_PORT, DS_PIN, gpd_output | gpot_pushpull | gpos_hi | gpaf_alter | (DS_ALTER_F<<gpaf_indx_bp) );  //

  DS_TIM_RCC_EN();

  DS_TIM->BDTR |= TIM_BDTR_MOE;

  DS_TIM->DIER = DS_IE_MASK;

  DS_TIM->DS_CMR_REG = DS_CMR_MASK;

  DS_TIM->PSC = DS_CPUSEC-1;

  DS_TIM->ARR = 0xffff;

  NVIC->ISER[DS_TIM_IRQ/32] = 1<<(DS_TIM_IRQ%32)  ;
}


void ds_req_pulse_long(uint16_t pulse_len) // us
{
  DS_TIM->CNT = 0xffff;

  DS_TIM->DS_IRQ_REG = DS_RESET_PULSE*2;

  DS_TIM->CCER = DS_CE_MASK_LONG;

  DS_TIM->DS_W_REG = pulse_len;

  DS_TIM->CR1 |= TIM_CR1_CEN;
}

void ds_req_pulse_short(uint16_t pulse_len) // us
{
  DS_TIM->CNT = 0xffff;

  DS_TIM->DS_IRQ_REG = DS_TIME_SLOT;

  DS_TIM->CCER = DS_CE_MASK_SHORT;

  DS_TIM->DS_W_REG = pulse_len;

  DS_TIM->CR1 |= TIM_CR1_CEN;
}

uint8_t ds_reset(void)
{
  ds_req_pulse_long(DS_RESET_PULSE);

  rtsWaitEvent();

  ds_ack_pulse = DS_TIM->DS_R_REG;

  return (ds_ack_pulse >= DS_RESET_PULSE);
}

void ds_write_bit(uint8_t lp_bit)
{
  if (lp_bit & 1) ds_req_pulse_short(DS_WR1_PULSE); else ds_req_pulse_short(DS_WR0_PULSE);

  rtsWaitEvent();
}

uint8_t ds_read_bit(void)
{
  ds_req_pulse_short(DS_RD_PULSE);

  rtsWaitEvent();

  ds_ack_pulse = DS_TIM->DS_R_REG;

  if (ds_ack_pulse > DS_RD_ACK_PULSE) return 0; else return 1;
}

void ds_write_byte(uint8_t lp_value)
{
        uint8_t i;
        for (i=0; i<8; i++) {ds_write_bit(lp_value); lp_value >>= 1; }
}

uint8_t ds_read_byte(void)
{
        uint8_t i;
        uint8_t temp;

        temp = 0;

        for (i=0; i<8; i++) {temp >>= 1;  temp |= (ds_read_bit()<<7) ; }

        return temp;
}

void ds_start_T_conv(void)
{
}

void ds_conv_delay(uint16_t conv_delay)
{
}

void ds_read_scratch(void )
{
}

uint8_t ds_check_crc(void )
{
        return 0;
}



void ds_pin_pushpull(void)
{
  gpioCFG(DS_PORT, DS_PIN, gpd_output | gpot_pushpull | gpos_hi | gpaf_alter | (DS_ALTER_F<<gpaf_indx_bp) );  //
}

void ds_pin_opendrain(void)
{
  gpioCFG(DS_PORT, DS_PIN, gpd_output | gpot_opendrain | gpos_hi | gpaf_alter | (DS_ALTER_F<<gpaf_indx_bp) );  //
}


void ds_manager(void)
{
  uint8_t i;

  while(1)
  {
    while(1)
    {
      ds_pin_opendrain();
      ds_present = ds_reset();
      if (!ds_present) {ds_temp = DS_NOT_PRESENT; break;}

      ds_pin_pushpull();
      ds_write_byte(DS_CMD_SKIP_ROM);
      ds_write_byte(DS_CMD_START_CONV);

      rtsDelay(800);

      ds_pin_opendrain();
      ds_present = ds_reset();
      if (!ds_present) {ds_temp = DS_NOT_PRESENT; break;}

      ds_pin_pushpull();
      ds_write_byte(DS_CMD_SKIP_ROM);
      ds_write_byte(DS_CMD_READ_SCRATCH_PAD);

            ds_pin_opendrain();
      for (i=0; i<9; i++) ds_scratch[i] = ds_read_byte();
      i = calcCRC8(ds_scratch, 8);
      if (i != ds_scratch[8]) {ds_temp = DS_CRC_ERROR; break;}

      ds_temp = ds_scratch[0] | (ds_scratch[1] << 8);

      ds_error = 0;
      if (ds_ready_handler) ds_ready_handler();
    }

    ds_error = 1;

    rtsDelay(20);
  }
}

int16_t ds_read_temp(void) {return ds_temp;}

#define ds_rts_stack_size 64
uint32_t ds_rts_stack[ds_rts_stack_size];


void ds_init(fptr l_h)
{
  ds_ready_handler = l_h;

  ds_low_level_init();

  ds_task_ID = rtsAddTask(&ds_manager, &ds_rts_stack[ds_rts_stack_size-1], 1);
}
