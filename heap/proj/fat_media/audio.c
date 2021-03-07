
#include "system.h"
#include <string.h>
#include "pins.h"
#include "media.h"
#include "audio.h"

#define timF         44100
#define timPrescaler 1
#define timPSC (timPrescaler-1)
#define timARR (F_APB1_TIM/timPrescaler/timF)

#define outDAC 0
#define outTIM 1


static uint8_t outtype;

void audioDacCgf(mediaFormatType* fmt)
{
  uint8_t bps;

  bps = mediaGetBytesSample(fmt);

  if (bps > 4) return;

  if (fmt->chann == 1)
  {
    if      (fmt->bitd == 8 ) DMA2_Channel3->CPAR = (uint32_t) &DAC->DHR8R1;
    else if (fmt->bitd == 16) DMA2_Channel3->CPAR = (uint32_t) &DAC->DHR12L1;

    DAC->CR = 0x00*DAC_CR_TSEL1_0 | DAC_CR_TEN1 | DAC_CR_EN1 | DAC_CR_DMAEN1;
  }
  else if (fmt->chann == 2)
  {
    if      (fmt->bitd == 8 ) DMA2_Channel3->CPAR = (uint32_t) &DAC->DHR8RD;
    else if (fmt->bitd == 16) DMA2_Channel3->CPAR = (uint32_t) &DAC->DHR12LD;

    DAC->CR = 0x00*DAC_CR_TSEL1_0 | DAC_CR_TEN1 | DAC_CR_EN1 | DAC_CR_EN2 | DAC_CR_DMAEN1;
  }

  if (bps == 1)
    DMA2_Channel3->CCR = DMA_CCR_MINC |  DMA_CCR_PSIZE_0*0x0 | DMA_CCR_MSIZE_0*0x0 | DMA_CCR_DIR  | DMA_CCR_CIRC ;//| DMA_CCR_TCIE;
  else if (bps == 2)
    DMA2_Channel3->CCR = DMA_CCR_MINC |  DMA_CCR_PSIZE_0*0x1 | DMA_CCR_MSIZE_0*0x1 | DMA_CCR_DIR  | DMA_CCR_CIRC ;//| DMA_CCR_TCIE;
  else if (bps == 4)
    DMA2_Channel3->CCR = DMA_CCR_MINC |  DMA_CCR_PSIZE_0*0x2 | DMA_CCR_MSIZE_0*0x2 | DMA_CCR_DIR  | DMA_CCR_CIRC ;//| DMA_CCR_TCIE;
}

void audioOutCgf(mediaFormatType* fmt)
{
  if (outtype == outDAC) audioDacCgf(fmt);
  else if (outtype == outTIM) {}
}


void audioSetSampleFreq(uint32_t freq)
{
  if (outtype == outDAC)
  {
    TIM6->ARR  = (F_APB1_TIM/timPrescaler)/freq;

    TIM6->PSC  = timPSC;
  } else if (outtype == outTIM)
  {
  }
}

void audioStartStream(void* buf, uint32_t size)
{
  if (outtype == outDAC)
  {
    DMA2_Channel3->CMAR = (uint32_t) buf;

    DMA2_Channel3->CNDTR = size;

    DMA2_Channel3->CCR |= DMA_CCR_EN;

    TIM6->CR1 |= TIM_CR1_CEN ;
  } else if (outtype == outTIM)
  {
  }
}

void audioPlay(void* buf, uint32_t size, mediaFormatType* fmt)
{
  uint8_t bps;

  audioStop();

  bps = mediaGetBytesSample(fmt);

  memset(buf, 0, size);

  audioSetSampleFreq(fmt->freqs);

  audioOutCgf(fmt);

  audioStartStream(buf, size/bps);
}

void audioPause(void)
{
  if (outtype == outDAC) TIM6->CR1 &= ~TIM_CR1_CEN ;
  else if (outtype == outTIM) {} ;
}

void audioStop(void)
{
  if (outtype == outDAC)
  {
    TIM6->CR1 &= ~TIM_CR1_CEN ;

    DMA2_Channel3->CCR &= ~DMA_CCR_EN;
  } else if (outtype == outTIM)
  {
  }
}

void audioContinue(void)
{
  if (outtype == outDAC) TIM6->CR1 |= TIM_CR1_CEN;
  else if (outtype == outTIM) {};
}

uint32_t audioPlayCounter(mediaFormatType* fmt)
{
  uint8_t bps;

  bps = mediaGetBytesSample(fmt);

  if (outtype == outDAC) return DMA2_Channel3->CNDTR * bps;
  else if (outtype == outTIM) return 0;

        return 0;
}

void audioInit(TIM_TypeDef* timl, TIM_TypeDef* timr)
{
  if (timl == 0) outtype = outDAC; else outtype = outTIM;

  if (outtype == outDAC)
  {
    RCC->APB1ENR |= RCC_APB1ENR_DAC1EN;
    RCC->AHBENR  |= RCC_AHBENR_DMA2EN;
    RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;

    TIM6->PSC  = timPSC;
    TIM6->ARR  = timARR;

    TIM6->CR2  = 0x02*TIM_CR2_MMS_0;
    TIM6->CR1  = TIM_CR1_URS;

  } else if (outtype == outTIM)
  {
  }
}

