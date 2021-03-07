
#ifndef _HK_AUDIO_H
#define _HK_AUDIO_H

#include "system.h"
#include "media.h"

void audioPlay(void* buf, uint32_t size, mediaFormatType* fmt);
void audioPause(void);
void audioStop(void);
void audioContinue(void);
uint32_t audioPlayCounter(mediaFormatType* fmt);
void audioInit(TIM_TypeDef* timl, TIM_TypeDef* timr);

#endif //_HK_AUDIO_H



