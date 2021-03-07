
#ifndef _HK_MEDIA_H
#define _HK_MEDIA_H

#include "system.h"
#include "ff.h"

#define mdr_OK            0
#define mdr_FILE_ERROR    1
#define mdr_FORMAT_ERROR  2


#pragma pack(push,1)

typedef struct {
  uint32_t chunkID;
  uint32_t chunkSize;
  uint32_t format;
} riff_head_t;

typedef struct {
  uint32_t subchunk1ID;
  uint32_t subchunk1Size;
} riff_chunk_t;

typedef struct{
  uint16_t audioFormat;
  uint16_t numChannels;
  uint32_t sampleRate;
  uint32_t byteRate;
  uint16_t blockAlign;
  uint16_t bitsPerSample;
  uint16_t cbSize;
} pcm_format_t;

typedef struct{
  uint32_t subchunk2ID;
  uint32_t subchunk2Size;
} pcm_data_t;

#pragma pack(pop)

typedef struct{
uint8_t chann;
uint32_t freqs;
uint8_t bitd;
} mediaFormatType;


uint8_t mediaGetBytesSample(mediaFormatType* fmt);
void mediaDecodeBlock(int16_t* buf, uint16_t size, mediaFormatType* mft);
uint8_t mediaReadFileHeader(FIL* mediaFile, mediaFormatType* mformat);

#endif // _HK_MEDIA_H

