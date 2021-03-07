
#include "system.h"
#include "pins.h"
#include "ff.h"
#include <stdio.h>
#include <string.h>


#include "ff_gen_drv.h"
#include "sd_diskio.h"

#include "console.h"
#include "audio.h"
#include "media.h"
#include "app_fat.h"


#include "futils.c"

#define MEDIA_BLOCK_SIZE 20000//8192 //16384
#define VIS_DIV_CONST    20
#define KB_DELAY         20
#define WAIT_DELAY       4
#define FAT_STR_MAX      80

uint8_t media_block[MEDIA_BLOCK_SIZE];

void fat_main(void);


FATFS             SDFatFs;   // File system object for SD card logical drive
FIL               mFile;     // File object
char              SDPath[4]; // SD card logical drive path
mediaFormatType   mFormat;
char              fat_str[FAT_STR_MAX];
uint8_t           fat_file_correct = 0;
uint8_t           fat_rtsh;
uint8_t           reshd;


void Error_Handler(const uint8_t err_msg[80])
{
  pin_set(ledPORT, errorLED);
  printf("%s%s", err_msg,"\r");

  while(1) rtsDelay(KB_DELAY);
}

void fat_init(void)
{
  fat_rtsh = rtsAddTask(&fat_main, 1);

  fat_set_rts(fat_rtsh);

  con_Init();
	audioInit(0, 0);
}

void mediaStreamFileRead(void)
{
  FRESULT   res;
  UINT      read_size;
	void*  bufp;
	uint32_t  remb;

  uint8_t   vis_div = VIS_DIV_CONST;
  uint32_t  bcnt    = 0;
  uint8_t   hblock  = 0;

  while ( f_eof(&mFile) == 0 )
  {

    if (con_kbpressed() )
    {
      if (con_kbcode() == ' ')
      {
        audioPause();

        while ( !con_kbpressed() ) rtsDelay(KB_DELAY);

        audioContinue();
      }

      if (con_kbcode () == 'b') break;
    }
		
		remb = audioPlayCounter(&mFormat);
		
//		if (cntb => MEDIA_BLOCK_SIZE/2) {pblock = 1; cntb -= MEDIA_BLOCK_SIZE/2;}
//		  else pblock = 0;

    if ( (hblock == 0) && (remb < MEDIA_BLOCK_SIZE/2) )
    {
      bufp = &media_block[0];

      hblock = 1;
    } else

    if ( (hblock == 1) && (remb > MEDIA_BLOCK_SIZE/2) )
    {
      bufp = &media_block[MEDIA_BLOCK_SIZE/2];

      hblock = 0;

//    } else {rtsDelay(WAIT_DELAY); continue;}  //rtsDelay(WAIT_DELAY);
    } else {rtsSkip(); continue;}  //rtsDelay(WAIT_DELAY);

    pin_set(ledPORT, block_readLED);

    res = f_read(&mFile, bufp, MEDIA_BLOCK_SIZE/2, (UINT*)&read_size);

    if((read_size == 0) || (res != FR_OK))  { Error_Handler("file read Error"); }

    mediaDecodeBlock(bufp, read_size, &mFormat);

    pin_clr(ledPORT, block_readLED);

    bcnt++;

    if (vis_div-- == 0) {printf("%s%u%s", "blocks: ", bcnt, "\r"); vis_div = VIS_DIV_CONST;}
  }

  audioStop();
}



void fat_main(void)
{
  printf("%s", "FAT FS + Media Entry\r");

  printf("%s", "Driver installation..\r");
        if(FATFS_LinkDriver(&SD_Driver, SDPath) != 0) { FATFS_UnLinkDriver(SDPath);  Error_Handler("drv Link Error"); }

  printf("%s", "Mounting drive..\r");
  if(f_mount(&SDFatFs, (TCHAR const*)SDPath, 0) != FR_OK) { Error_Handler("FatFs Initialization Error"); }


  while (1)
  {
    pin_clr(ledPORT, fatLED);

    while (1)
    {
      printf("%s", "Enter command: (dir/play) \r");

      con_read_str(fat_str);

      if (strcmp(fat_str,"dir") == 0) fat_read_dir(SDPath);

      if (strcmp(fat_str,"play") == 0) break;
    }

    fat_file_correct = 0;

    while (!fat_file_correct)
    {
      while (1)
      {
        printf("%s", "Enter filename\r");

        con_read_str(fat_str);

        if (f_stat(fat_str, 0) != FR_OK) printf("%s%s%s", "FileName: ",fat_str, " incorrect\r");
         else   break;
      }

      if(f_open(&mFile, fat_str, FA_READ) != FR_OK) { Error_Handler("file open Error"); }

      printf("%s%s%s", "File: ",fat_str, " open\r");

      reshd = mediaReadFileHeader(&mFile, &mFormat);

      if ( reshd != mdr_OK )
      {
        f_close(&mFile);

        if ( reshd == mdr_FILE_ERROR   ) printf("%s", "File read error\r");
        if ( reshd == mdr_FORMAT_ERROR ) printf("%s", "Incorrect File format\r");

        continue;
      } else fat_file_correct = 1;
    }

    printf("%s", "FileFormat: correct WAV\r");

    printf("%s%u\r", "Channels number: ", mFormat.chann);
    printf("%s%u%s\r", "SampleRate: ",    mFormat.freqs, " Hz");
    printf("%s%u\r", "Bits per sample: ", mFormat.bitd);

    printf("%s", "Press key to play\r");
    while ( !con_kbpressed() ) rtsDelay(KB_DELAY);

    printf("%s", "Playing... (space->pause, b->break)\r");

    audioPlay(media_block, MEDIA_BLOCK_SIZE, &mFormat);

    mediaStreamFileRead();

    f_close(&mFile);

    pin_set(ledPORT, fatLED);

    printf("%s", "Playing complette\r");
    printf("%s", "Press key to continue\r");
    while (!con_kbpressed() ) rtsDelay(KB_DELAY);
  }

//  FATFS_UnLinkDriver(SDPath);
//  while(1) rtsDelay(50);
}
