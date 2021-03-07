//------------------------------------------------------------------
//------------------------------------------------------------------
static void spiDrvSendPack(uint8_t* lDrvTxBuf, uint8_t lDrvSize)
{
    _spix_TX_DMA->CCR  &= ~DMA_CCR_EN;
    _spix_TX_DMA->CNDTR = lDrvSize;
    _spix_TX_DMA->CMAR  = (uint32_t) lDrvTxBuf;
    _spix_TX_DMA->CCR  |= DMA_CCR_EN;
}
//------------------------------------------------------------------
//------------------------------------------------------------------
#ifdef spiUSE_RX
void spiDrvResetRx(uint8_t* lDrvRxBuf, uint8_t lDrvSize)
{
    _spix_RX_DMA->CCR  &= ~DMA_CCR_EN;
    _spix_RX_DMA->CNDTR = lDrvSize;
    _spix_RX_DMA->CMAR  = (uint32_t) lDrvRxBuf;
    _spix_RX_DMA->CCR  |= DMA_CCR_EN;
}
#endif
//------------------------------------------------------------------
//------------------------------------------------------------------
void spiDrvInit(void)
{
        RCC->AHBENR  |= RCC_AHBENR_DMA1EN;
        RCC->_spix_APB_EN |= _spix_APB_MASK;

        _spixPORT->CR2 = SPI_CR2_RXDMAEN | SPI_CR2_TXDMAEN;

        _spixPORT->CR1 =  (spiMODE << SPI_CR1_CPHA) | SPI_CR1_MSTR | \
                           spiPSCMask(spiPSC)*SPI_CR1_BR_0 | \
                                 spiLSB_FIRST*SPI_CR1_LSBFIRST | \
                                 SPI_CR1_SSI | SPI_CR1_SSM;


        _spix_TX_DMA->CCR = DMA_CCR_MINC |  DMA_CCR_PSIZE_1 | DMA_CCR_DIR;
        _spix_TX_DMA->CPAR = (uint32_t) &_spixPORT->DR;

#ifdef spiUSE_RX
        _spix_RX_DMA->CCR = DMA_CCR_MINC |  DMA_CCR_PSIZE_1;
        _spix_RX_DMA->CPAR = (uint32_t) &_spixPORT->DR;
#endif

        _spixPORT->CR1 |= SPI_CR1_SPE;
}
//------------------------------------------------------------------
//------------------------------------------------------------------
