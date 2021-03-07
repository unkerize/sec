
#define spiSize 4

uint8_t  spi_tx_buffer[spiSize];
uint8_t  spi_rx_buffer[spiSize];

uint8_t  spi0Inst;

//#define spiOSStyle spioOsNON
#define spiOSStyle spioOsST
//#define spiOSStyle spioOsRTS



#if spiOSStyle == spioOsRTS
void spiTask(void)
{
	while(1)
	{
	spi_tx_buffer[0] = 1;
	spi_tx_buffer[1] = 0x40;
	spiTxBlock(spi0Inst, spi_tx_buffer, spi_rx_buffer, spiSize); //spiSize
	
	rtsWaitEvent();
	
  pin_set(ledPORT, led2);
	rtsDelay(2);
  pin_clr(ledPORT, led2);
	rtsDelay(2);
	}
}
#endif


#if spiOSStyle == spioOsST
void spiTask(void)
{
	spi_tx_buffer[0] = 1;
	spi_tx_buffer[1] = 0x40;
	spiTxBlock(spi0Inst, spi_tx_buffer, spi_rx_buffer, spiSize); 
}
#endif


#if spiOSStyle == spioOsNON

uint8_t some;
void spiTask(void)
{

	if (!spiBusy(spi0Inst))
	{
	spi_tx_buffer[0] = 1;
	spi_tx_buffer[1] = 0x40;
//	spiTxBlock(spi0Inst, spi_tx_buffer, spi_rx_buffer, spiSize); 
	spiSetDummy(spi0Inst, 0x01);
	spiTxBlock(spi0Inst, 0, 0, spiSize); 
	}

//  some = spiTxByte(spi0Inst, some);	
}
#endif


void spiInstall(void)
{
#if spiOSStyle == spioOsRTS
  spi0Inst =  spiAdd(spiPORT, spioMODE0 | spioMSBF | spioBR32 | spiOSStyle, rtsAddTask(&spiTask, 1));
#endif
	
#if spiOSStyle == spioOsST
  spi0Inst =  spiAdd(spiPORT, spioMODE0 | spioMSBF | spioBR32 | spiOSStyle, stAddTask(&spiTask, stf_event));
	spiTask();
#endif	
	
	
#if spiOSStyle == spioOsNON
  spi0Inst =  spiAdd(spiPORT, spioMODE0 | spioMSBF | spioBR32 | spiOSStyle, 0xff);
	stAddTask(&spiTask,stf_ring);
#endif	
}
