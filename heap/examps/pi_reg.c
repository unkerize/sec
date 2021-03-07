int32_t integ = 0;
int32_t adc_ref = 1500;
int32_t power;

void pi_reg(void)
{
	int32_t val;
	int32_t diff;
	int32_t pwm_t;
	uint16_t pwm;
	
	val = adc_buff[1] - adc_buff[0];
	
	if (val < 0) val = 0;
	
	diff = adc_ref - val  ;
	
	integ = integ + diff;
	
	if (integ < -4095*10) integ = -4095*10;
	if (integ > 4095*10)  integ = 4095*10;
	
	if (integ < 0) power = 0; else power = integ >> 3;
	
	if (power > 4095) pwm_t = pwm_tim_arr;
	 else	pwm_t = power*pwm_tim_arr/4096;

	pwm = pwm_t;
	
	tim_pwm1_set(TIMER, pwm);
}

#define FAN_START_VOLTAGE 1500
#define TEMP_LOW  30
#define TEMP_HIGH 60
#define TEMP_SCALE (TEMP_HIGH-TEMP_LOW)

void tep_reg(void)
{
	int16_t temp;
	int16_t fan_scale;
	
	temp = ds_read_temp(); 
	
	if ( (temp == DS_NOT_PRESENT) || (temp == DS_CRC_ERROR) )
	{
		adc_ref = adc_buff[1];
		return;
	}
	
	if (temp < 0) temp = 0;
	
	temp = temp >> 4;
	
	if (temp <= TEMP_LOW) {adc_ref = 0; return;}
	
	if (temp > TEMP_HIGH) {adc_ref = adc_buff[1]; return;}
	
	fan_scale = adc_buff[1] - FAN_START_VOLTAGE;
	
	adc_ref = fan_scale*(temp - TEMP_LOW)/TEMP_SCALE + FAN_START_VOLTAGE;
	
	if (adc_ref > adc_buff[1]) adc_ref = adc_buff[1];
}
