
uint8_t uart_h;
uint8_t rx_buf[16];
uint8_t tx_buf[128];

void fill_str(uint8_t* out)
{
	  out[5] = '-';
    out[4] = '-' ;
    out[3] = '-' ;
    out[2] = '-' ;
    out[1] = '-' ;
    out[0] = '-' ;
	
	  out[6] = 10;
	  out[7] = 13;
}


void dec2str(uint16_t d, uint8_t* out)
{
    out[4] = '0' + ( d       )    % 10;
    out[3] = '0' + ( d /= 10 )    % 10;
    out[2] = '0' + ( d /= 10 )    % 10;
    out[1] = '0' + ( d /= 10 )    % 10;
    out[0] = '0' + ( d /  10 )    % 10;
	
	  out[5] = 10;
	  out[6] = 13;
}

void decs2str(int16_t d, uint8_t* out)
{
  if (d < 0)
	{
		out[0] = '-';
		d = -d;
	} else out[0] = ' ';
		
    out[5] = '0' + ( d       )    % 10;
    out[4] = '0' + ( d /= 10 )    % 10;
    out[3] = '0' + ( d /= 10 )    % 10;
    out[2] = '0' + ( d /= 10 )    % 10;
    out[1] = '0' + ( d /  10 )    % 10;
	
	  out[6] = 10;
	  out[7] = 13;
}



void tx_hndl(void)
{
//      uint8_t but;

//  decs2str(ds_read_temp() >> 4 ,&tx_buf[0]);
//      but = pin_read(GPIOA,0) == 0 ;

  decs2str(adc_buff[0] ,&tx_buf[8]);
//  decs2str(adc_buff[1] ,&tx_buf[16]);
//  decs2str(power ,&tx_buf[24]);
//  decs2str(ds_read_debug() ,&tx_buf[32]);


        fill_str(&tx_buf[40]);

//  decs2str(12345 ,&tx_buf[0]);
//  decs2str(-12345 ,&tx_buf[8]);

  uartTxBlock(uart_h, tx_buf, 8*6);
}


void conn_uart(void)
{

uart_h = uartAdd(   USART1,
                   115200,
                   0,
                   rx_buf,
                   16);

  stAddTask(&tx_hndl,stf_1Hz);
}

