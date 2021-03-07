//------------------------------------------------------------------
//------------------------------------------------------------------
#define spiTxDmaBlockMask (DMA_CCR_MINC |  DMA_CCR_PSIZE_1*0 | DMA_CCR_DIR  | DMA_CCR_EN)
#define spiTxDmaDummyMask (           0 |  DMA_CCR_PSIZE_1*0 | DMA_CCR_DIR  | DMA_CCR_EN)
#define spiRxDmaBlockMask (DMA_CCR_MINC |  DMA_CCR_PSIZE_1*0 | DMA_CCR_TCIE | DMA_CCR_EN)
#define spiRxDmaDummyMask (           0 |  DMA_CCR_PSIZE_1*0 | DMA_CCR_TCIE | DMA_CCR_EN)

//------------------------------------------------------------------
// common drv code
#include "spi_drv.c"
//------------------------------------------------------------------

// SPI1
void DMA1_Channel2_IRQHandler(void)
{
  DMA1->IFCR = DMA_IFCR_CTCIF2;

  spiSetEv(spi_handler[0]);
}

// SPI2
void DMA1_Channel4_IRQHandler(void)
{
  DMA1->IFCR = DMA_IFCR_CTCIF4;

  spiSetEv(spi_handler[1]);
}

#ifdef RCC_APB1ENR_SPI3EN
// SPI3
void DMA2_Channel1_IRQHandler(void)
{
  DMA2->IFCR = DMA_IFCR_CTCIF1;

  spiSetEv(spi_handler[2]);
}
#endif

#ifdef RCC_APB2ENR_SPI4EN
// SPI4
void DMA2_Channel4_IRQHandler(void)
{
  DMA2->IFCR = DMA_IFCR_CTCIF4;

  spiSetEv(spi_handler[3]);
}
#endif


void SPI1_IRQHandler(void)
{
  SPI1->DR;
  SPI1->CR2 &= ~SPI_CR2_RXNEIE;
  spiSetEv(spi_handler[0]);
}

void SPI2_IRQHandler(void)
{
//  temp = SPI2->DR;
//        temp = *(__IO uint8_t *)&SPI2->DR;
  SPI2->DR;
  SPI2->CR2 &= ~SPI_CR2_RXNEIE;
  spiSetEv(spi_handler[1]);
}

#ifdef RCC_APB1ENR_SPI3EN
void SPI3_IRQHandler(void)
{
  SPI3->DR;
  SPI3->CR2 &= ~SPI_CR2_RXNEIE;
  spiSetEv(spi_handler[2]);
}
#endif

#ifdef RCC_APB2ENR_SPI4EN
void SPI4_IRQHandler(void)
{
  SPI4->DR;
  SPI4->CR2 &= ~SPI_CR2_RXNEIE;
  spiSetEv(spi_handler[3]);
}
#endif

uint8_t spiAdd(SPI_TypeDef* lspi_port, uint32_t lspi_options)
{
  uint8_t spiMODE;
  uint8_t spiBR;
  uint8_t spiMSBF;
  IRQn_Type spiIRQ_DMA;
  IRQn_Type spiIRQ;

  spi[spiCounter].busy = 0;
  spi[spiCounter].osComOpt =   (lspi_options >> spioOScom_bp) & spioOScom_msk;
  spi[spiCounter].osComIndex = (lspi_options >> spioOSindx_bp) & spioOSindx_msk;

  spiMODE = (lspi_options >> spioMODE_bp) & spioMODE_msk;
  spiBR   = (lspi_options >> spioBR_bp) & spioBR_msk;
  spiMSBF = (lspi_options >> spioMSBF_bp) & spioMSBF_msk;

  spi[spiCounter].port = lspi_port;
  spi[spiCounter].wDummy = 0xff;
  spi[spiCounter].rDummy = 0x00;

  if (lspi_port == SPI1)
  {
    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
    RCC->AHBENR  |= RCC_AHBENR_DMA1EN;

    spi[spiCounter].txDma = DMA1_Channel3;
    spi[spiCounter].rxDma = DMA1_Channel2;

    spiIRQ_DMA = DMA1_Channel2_IRQn;
    spiIRQ     = SPI1_IRQn;

    spi_handler[0] = spiCounter;
  }

  if (lspi_port == SPI2)
  {
    RCC->APB1ENR |= RCC_APB1ENR_SPI2EN;
    RCC->AHBENR  |= RCC_AHBENR_DMA1EN;

    spi[spiCounter].txDma = DMA1_Channel5;
    spi[spiCounter].rxDma = DMA1_Channel4;

    spiIRQ_DMA = DMA1_Channel4_IRQn;
    spiIRQ     = SPI2_IRQn;

    spi_handler[1] = spiCounter;
  }

#ifdef RCC_APB1ENR_SPI3EN
  if (lspi_port == SPI3)
  {
    RCC->APB1ENR |= RCC_APB1ENR_SPI3EN;
    RCC->AHBENR  |= RCC_AHBENR_DMA2EN;

    spi[spiCounter].txDma = DMA2_Channel2;
    spi[spiCounter].rxDma = DMA2_Channel1;

    spiIRQ_DMA = DMA2_Channel1_IRQn;
    spiIRQ     = SPI3_IRQn;

    spi_handler[2] = spiCounter;
  }
#endif

#ifdef RCC_APB2ENR_SPI4EN
  if (lspi_port == SPI4)
  {
    RCC->APB2ENR |= RCC_APB2ENR_SPI4EN;
    RCC->AHBENR  |= RCC_AHBENR_DMA2EN;

    spi[spiCounter].txDma = DMA2_Channel5;
    spi[spiCounter].rxDma = DMA2_Channel4;

    spiIRQ_DMA = DMA2_Channel4_IRQn;
    spiIRQ     = SPI4_IRQn;

    spi_handler[3] = spiCounter;
  }
#endif

  spi[spiCounter].port->CR2 = SPI_CR2_RXDMAEN | SPI_CR2_TXDMAEN | SPI_CR2_FRXTH | (7*SPI_CR2_DS_0);
//  spi[spiCounter].port->CR2 =  SPI_CR2_FRXTH | (7*SPI_CR2_DS_0);

  spi[spiCounter].port->CR1 = (spiMODE*SPI_CR1_CPHA) | SPI_CR1_MSTR | \
                            spiBR*SPI_CR1_BR_0 |                      \
                            spiMSBF*SPI_CR1_LSBFIRST |                \
                            SPI_CR1_SSI | SPI_CR1_SSM;

  if (spi[spiCounter].txDma->CCR != 0) sysErrHandler(ser_dma_cross);
  if (spi[spiCounter].rxDma->CCR != 0) sysErrHandler(ser_dma_cross);
  spi[spiCounter].txDma->CCR = DMA_CCR_PL_0; // mark dma
  spi[spiCounter].rxDma->CCR = DMA_CCR_PL_0; // mark dma

  spi[spiCounter].txDma->CPAR = (uint32_t) &spi[spiCounter].port->DR;

  spi[spiCounter].rxDma->CPAR = (uint32_t) &spi[spiCounter].port->DR;

  NVIC_EnableIRQ(spiIRQ_DMA);
  NVIC_EnableIRQ(spiIRQ);

//  spi[spiCounter].port->I2SCFGR = SPI_I2SCFGR_I2SMOD | SPI_I2SCFGR_I2SE;

  spi[spiCounter].port->CR1 |= SPI_CR1_SPE;

//  spi[spiCounter].port->DR = 0xffffffff; //(uint16_t) 0xffff;
//  while ( spi[spiCounter].port->SR & SPI_SR_BSY) {};

  return(spiCounter++);
}

//------------------------------------------------------------------
void spiTxBlock(uint8_t lSpi, uint8_t* ltxBuf, uint8_t* lrxBuf, uint16_t lSize)
{
  if (lSize == 0) return;

  spi[lSpi].busy = 1;
  //-----------------------------------------------------------
  // reset RX
  spi[lSpi].rxDma->CCR   = 0;
  spi[lSpi].rxDma->CNDTR = lSize;

  if (lrxBuf)
  {
    spi[lSpi].rxDma->CMAR  = (uint32_t) lrxBuf;
    spi[lSpi].rxDma->CCR   = spiRxDmaBlockMask;
  } else
  {
    spi[lSpi].rxDma->CMAR  = (uint32_t) &spi[lSpi].rDummy;
    spi[lSpi].rxDma->CCR   = spiRxDmaDummyMask;
  }
  //-----------------------------------------------------------
  // reset TX
  spi[lSpi].txDma->CCR   = 0;
  spi[lSpi].txDma->CNDTR = lSize;

  if (ltxBuf)
  {
    spi[lSpi].txDma->CMAR  = (uint32_t) ltxBuf;
    spi[lSpi].txDma->CCR   = spiTxDmaBlockMask;
  } else
  {
    spi[lSpi].txDma->CMAR  = (uint32_t) &spi[lSpi].wDummy;
    spi[lSpi].txDma->CCR   = spiTxDmaDummyMask;
  }
}
//------------------------------------------------------------------
//------------------------------------------------------------------
uint8_t spiTxByte(uint8_t spih, uint8_t txValue)
{
  while (spi[spih].port->SR & SPI_SR_BSY) {};
  *(__IO uint8_t *) &spi[spih].port->DR = txValue;
//  while (spi[spih].port->SR & SPI_SR_BSY) {};
  while ( (spi[spih].port->SR & SPI_SR_RXNE) != SPI_SR_RXNE ) {};
  return (uint8_t) spi[spih].port->DR;
}
//------------------------------------------------------------------
//------------------------------------------------------------------
uint8_t spiTxByteAsync(uint8_t spih, uint8_t txValue)
{
  while (spi[spih].port->SR & SPI_SR_BSY) {};

  spi[spih].busy = 1;

//  (uint8_t) spi[spih].port->DR;

  spi[spih].port->CR2 |= SPI_CR2_RXNEIE;

  *(__IO uint8_t *) &(spi[spih].port->DR) = txValue;

  rtsWaitEvent();

  return *(__IO uint8_t *)&spi[spih].port->DR;
//  return temp;
}
//------------------------------------------------------------------
//------------------------------------------------------------------
