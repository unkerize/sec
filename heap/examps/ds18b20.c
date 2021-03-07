//------------------------------------------------------------------------------------------
// DS18B20

fptr    ds_irq_h;
uint8_t ds_buf[10];

//void TIM1_BRK_UP_TRG_COM_IRQHandler(void)
void TIM1_CC_IRQHandler (void)
{
  ds_irq_h();
//  pin_xor(led2Port, led2Pin);

}


void ds_pre(void)
{
//pin_xor(led2Port, led2Pin);
pin_set(led2Port, led2Pin);
}

void ds_post(void)
{
pin_clr(led2Port, led2Pin);
}


void conn_ds(void)
{
  NVIC_EnableIRQ(TIM1_CC_IRQn);

ds_cfg_td ds_cfg;

  ds_cfg.tim = TIM1 ;
  ds_cfg.chn = 3 ;

ds_cfg.port = GPIOA;
ds_cfg.pin  = 11;
ds_cfg.alterf  = 2; // F030-2, F303-11

ds_cfg.pre_handler = &ds_pre;
ds_cfg.post_handler = &ds_post;

ds_irq_h = ds_init(&ds_cfg);
}


	ntsTxBuf[2] = ds_get_type();
	
	ntsTxBuf[3] = ds_count();
	
	ntsTxBuf[4] = ds_cmd_ready();
	
	ntsTxBuf[5] = ds_get_multi();
