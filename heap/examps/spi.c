//uint8_t spi_inst;
//#define spi_buf_size 2
//uint8_t spiTxBuf[spi_buf_size];
//uint8_t spiRxBuf[spi_buf_size];



uint8_t seg_counter = 0;
uint8_t seg_buf[4];



const uint8_t seg_mask[4] = { ~(1<<6), ~(1<<5), ~(1<<3), ~(1<<2)};

/*
void spi_tick(void)
{
  pin_set(ltc_port, ltc_pin);
	syncDelay(2);
  pin_clr(ltc_port, ltc_pin);
	
	seg_buf[0] = 1;
	seg_buf[1] = 2;
	seg_buf[2] = 4;
	seg_buf[3] = 8;
	
	spiTxBuf[0] = seg_mask[seg_counter]  & ~(((spiRxBuf[0]>>2) & 1) <<1) & ~(((spiRxBuf[0]>>3) & 1) <<4);
	spiTxBuf[1] = ~seg_buf[seg_counter];
	
	spiTxBlock(spi_inst, spiTxBuf, spiRxBuf, spi_buf_size);
	
	if (++seg_counter == 4) seg_counter = 0;
	
}
*/

