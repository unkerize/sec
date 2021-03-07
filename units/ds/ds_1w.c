
//#if defined(STM32F10X)
  __IO uint16_t* ds_req_pulse_reg;
  __IO uint16_t* ds_ack_pulse_reg;
  __IO uint16_t* ds_timing_reg;
//#endif

// chn = 0
// 1 - req_pulse
// 2 - ack_pulse
// 3 - timing
// 4 - not used

// chn = 1
// 1 - ack_pulse
// 2 - req_pulse
// 3 - timing
// 4 - not used

// chn = 2
// 1 - timing
// 2 - not used
// 3 - req_pulse
// 4 - ack_pulse

// chn = 3
// 1 - timing
// 2 - not used
// 3 - ack_pulse
// 4 - req_pulse

const uint16_t ds_mask_SR[4]  = { (uint16_t) ~TIM_SR_CC3IF, (uint16_t) ~TIM_SR_CC3IF, (uint16_t) ~TIM_SR_CC1IF, (uint16_t) ~TIM_SR_CC1IF};

const uint16_t ds_mask_IE[4]  = {TIM_DIER_CC3IE,TIM_DIER_CC3IE,TIM_DIER_CC1IE,TIM_DIER_CC1IE};

const uint16_t ds_mask_CMR[4] = {
  (TIM_CCMR1_CC1S_0*0 | TIM_CCMR1_OC1M_0*6)  | TIM_CCMR1_CC2S_0*2,
  (TIM_CCMR1_CC2S_0*0 | TIM_CCMR1_OC2M_0*6)  | TIM_CCMR1_CC1S_0*2,
  (TIM_CCMR2_CC3S_0*0 | TIM_CCMR2_OC3M_0*6)  | TIM_CCMR2_CC4S_0*2,
  (TIM_CCMR2_CC4S_0*0 | TIM_CCMR2_OC4M_0*6)  | TIM_CCMR2_CC3S_0*2
 };

const uint16_t ds_mask_CER[4]  = {
(TIM_CCER_CC2E | TIM_CCER_CC2P*0) | (TIM_CCER_CC1E | TIM_CCER_CC1P*1),
(TIM_CCER_CC2E | TIM_CCER_CC2P*1) | (TIM_CCER_CC1E | TIM_CCER_CC1P*0),
(TIM_CCER_CC4E | TIM_CCER_CC4P*0) | (TIM_CCER_CC3E | TIM_CCER_CC3P*1),
(TIM_CCER_CC4E | TIM_CCER_CC4P*1) | (TIM_CCER_CC3E | TIM_CCER_CC3P*0)
};


void ds_irq_handler(void)
{
  ds_tim->CR1 &= ~TIM_CR1_CEN;

  ds_tim->SR &= ds_mask_SR[ds_chn];

  rtsSetEvent(ds_task_ID);
}

void ds_low_level_init(void)
{
  gpioCFG(ds_port,
          ds_pin,
          gpd_output | gpot_pushpull | gpos_hi | gpaf_alter |
          (ds_alterf<<gpaf_indx_bp) );

  tim_rcc_en(ds_tim);

  ds_tim->BDTR |= TIM_BDTR_MOE;

  ds_tim->DIER = ds_mask_IE[ds_chn];

  if (ds_chn < 2)
    ds_tim->CCMR1 = ds_mask_CMR[ds_chn];

  if (ds_chn > 1)
    ds_tim->CCMR2 = ds_mask_CMR[ds_chn];

  ds_tim->PSC = tim_apb_freq(ds_tim)/DS_MHZ-1;

  ds_tim->ARR = 0xffff;

  ds_tim->CCER = ds_mask_CER[ds_chn];
}

void ds_req_pulse_long(uint16_t pulse_len) // us
{
  ds_tim->CNT = 0xffff;

  *ds_timing_reg = DS_RESET_PULSE*2;

  *ds_req_pulse_reg = pulse_len;

  ds_tim->CR1 |= TIM_CR1_CEN;
}

void ds_req_pulse_short(uint16_t pulse_len) // us
{
  ds_tim->CNT = 0xffff;

  *ds_timing_reg  = DS_TIME_SLOT;

  *ds_req_pulse_reg = pulse_len;

  ds_tim->CR1 |= TIM_CR1_CEN;
}

uint8_t ds_reset(void)
{
  ds_req_pulse_long(DS_RESET_PULSE);

  rtsWaitEvent();

  return (*ds_ack_pulse_reg > DS_RESET_ACK_PULSE) ;
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

  if (*ds_ack_pulse_reg > DS_RD_ACK_PULSE) return 0; else return 1;
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

void ds_pin_pushpull(void)
{
  gpioCFG(ds_port,
          ds_pin,
          gpd_output | gpot_pushpull | gpos_hi | gpaf_alter |
          (ds_alterf<<gpaf_indx_bp) );
}

void ds_pin_opendrain(void)
{
  gpioCFG(ds_port,
          ds_pin,
          gpd_output | gpot_opendrain | gpos_hi | gpaf_alter |
          (ds_alterf<<gpaf_indx_bp) );
}

