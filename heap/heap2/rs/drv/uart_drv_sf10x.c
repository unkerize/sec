

static ntsData_t* rs;
static uint8_t rsRxEvID;

static void rsSendPack(uint8_t lSize)
{
    rs->txSize = lSize;

    _rsm_TX_DMA->CCR  &= ~DMA_CCR1_EN;
    _rsm_TX_DMA->CNDTR = rs->txSize;
    _rsm_TX_DMA->CMAR  = (uint32_t) &rs->txBuffer;
    _rsm_TX_DMA->CCR  |= DMA_CCR1_EN;
}

void rsResetRx(void)
{
    rs->rxCounter = (rsRxSize - _rsm_RX_DMA->CNDTR);

    rs->rxSize = rs->rxCounter;

    _rsm_RX_DMA->CCR &= ~DMA_CCR1_EN;
    _rsm_RX_DMA->CNDTR = rsRxSize;
    _rsm_RX_DMA->CMAR = (uint32_t) &rs->rxBuffer;
    _rsm_RX_DMA->CCR |= DMA_CCR1_EN;
}

void rsRxReadyEv(void)
{
    if (_rsmPORT->SR & USART_SR_ORE)
    {
        _rsmPORT->DR;
        rsResetRx();
        return;
    } else
    {
        rsResetRx();

        if (rs->rxHandler) rs->rxHandler();
    }
}

void rsInit(ntsData_t* lp_rs)
{
  rs = lp_rs;

#ifdef rs_485_PIN
  pin_clr(rs_485_PORT, rs_485_PIN);
#endif

        //-----------------------------------------------------------
        // init uart variables
        //-----------------------------------------------------------
        //-----------------------------------------------------------
        // init uart periph
        //-----------------------------------------------------------
        RCC->AHBENR  |= RCC_AHBENR_DMA1EN;

        RCC->_rsm_APB_EN |= _rsm_APB_MASK;

        _rsmPORT->BRR = rsBaudDiv << 4;
        _rsmPORT->CR3 |= USART_CR3_DMAT | USART_CR3_DMAR;

        _rsmPORT->CR1 = USART_CR1_TE | USART_CR1_RE | USART_CR1_IDLEIE;

        _rsmPORT->SR = 0;

        _rsm_TX_DMA->CCR = DMA_CCR1_MINC |  DMA_CCR1_PSIZE_1 | DMA_CCR1_DIR;
        _rsm_TX_DMA->CPAR = (uint32_t) &_rsmPORT->DR;

        _rsm_RX_DMA->CCR = DMA_CCR1_MINC |  DMA_CCR1_PSIZE_1;
        _rsm_RX_DMA->CPAR = (uint32_t) &_rsmPORT->DR;

        rsResetRx();

        NVIC->ISER[_rsm_IRQ/32] = 1<<(_rsm_IRQ%32)  ;

        _rsmPORT->CR1 |= USART_CR1_UE;

        rsRxEvID = stAddTask(&rsRxReadyEv, stf_event);
}

void _rsm_RX_Vector(void)
{
  if (_rsmPORT->SR & USART_SR_IDLE)
  {
    _rsmPORT->DR;

    if (rsRxSize - _rsm_RX_DMA->CNDTR) stSetEvent(rsRxEvID);
  }
}
