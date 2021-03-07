//------------------------------------------------------------------
// spi Drv common code
//------------------------------------------------------------------
// data
uint8_t spiCounter = 0;
spiType spi[spiNumber];
uint8_t spi_handler[spiMaxNumber];
//------------------------------------------------------------------
// funcs
void spiSetEv(uint8_t lspi)
{
  spi[lspi].busy = 0;

  if (spi[lspi].osComOpt == spiOsNON) return;

  if (spi[lspi].osComOpt == spiOsST)
  {
    stSetEvent(spi[lspi].osComIndex);
    return;
  }

  if (spi[lspi].osComOpt == spiOsRTS)
  {
    rtsSetEvent(spi[lspi].osComIndex);
    return;
  }
}


void spiSetDummy(uint8_t spih, uint8_t lDummy)
{
  spi[spih].wDummy = lDummy;
}


uint8_t spiBusy(uint8_t lSpi)
{
  return spi[lSpi].busy;
}
//------------------------------------------------------------------

