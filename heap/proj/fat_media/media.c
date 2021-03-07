
#include "system.h"
#include "pins.h"
#include "ff.h"
#include "media.h"

uint8_t mediaGetBytesSample(mediaFormatType* fmt)
{
   return fmt->chann*(fmt->bitd>>3);
}

void mediaDecodeBlock(int16_t* buf, uint16_t size, mediaFormatType* mft)
{
  uint16_t i;

  for (i = 0; i < size/2; i++) buf[i] = buf[i]+0x8000;
}


uint8_t mediaReadFileHeader(FIL* mediaFile, mediaFormatType* mformat)
{
  riff_head_t   riff_header;
  riff_chunk_t  riff_chunk;
  pcm_format_t  pcm_format;
  pcm_data_t    pcm_data;

  FRESULT res;
  UINT    read_size;

  res = f_read(mediaFile, &riff_header, sizeof(riff_header), &read_size);
  if((read_size == 0) || (res != FR_OK)) return mdr_FILE_ERROR;

  if (riff_header.chunkID != 0x46464952) return mdr_FORMAT_ERROR;
  if (riff_header.format  != 0x45564157) return mdr_FORMAT_ERROR;

  res = f_read(mediaFile, &riff_chunk, sizeof(riff_chunk), &read_size);
  if((read_size == 0) || (res != FR_OK)) return mdr_FILE_ERROR;

  if (riff_chunk.subchunk1ID != 0x20746D66) return mdr_FORMAT_ERROR;

  res = f_read(mediaFile, &pcm_format, riff_chunk.subchunk1Size, &read_size);
  if((read_size == 0) || (res != FR_OK)) return mdr_FILE_ERROR;

  if (pcm_format.audioFormat != 1) return mdr_FORMAT_ERROR;

  res = f_read(mediaFile, &pcm_data, sizeof(pcm_data), &read_size);
  if((read_size == 0) || (res != FR_OK)) return mdr_FILE_ERROR;

  mformat->chann = pcm_format.numChannels;
  mformat->freqs = pcm_format.sampleRate;
  mformat->bitd  = pcm_format.bitsPerSample;

  return mdr_OK;
}

