
#include "system.h"
#include "pins.h"
#include "ff.h"
#include "term.h"
#include <stdio.h>
#include <string.h>


#include "stm3210c_eval.h"

/* FatFs includes component */
#include "ff_gen_drv.h"
#include "sd_diskio.h"

void fat_work(void);
uint8_t fat_rtsh;


void fat_add(void)
{
  fat_rtsh = rtsAddTask(&fat_work, 1);
	
	fat_add_rts(fat_rtsh);
	
  terminalInit(fat_rtsh);
}


FATFS SDFatFs;  /* File system object for SD card logical drive */
FIL MyFile;     /* File object */
char SDPath[4]; /* SD card logical drive path */

void Error_Handler(void)
{
//   printf("%s", "Error\r");
//	while(1);
//   pin_set(ledPORT, led1);
}


void fat_work(void)
{

  FRESULT res;                                          /* FatFs function common result code */
  uint32_t byteswritten, bytesread;                     /* File write/read counts */
//  uint8_t wtext[] = "This is STM32 working with FatFs"; /* File write buffer */
//  uint8_t wtext[] = "WOW! I'm working with FAT FS!"; /* File write buffer */
//  uint8_t rtext[100];                                   /* File read buffer */
	
//	uint8_t termStr[100];
	TCHAR fileName[20];
	char wrStr[100];
	

   printf("%s", "HK_OS working\r");
   printf("%s", "FatFS Task started, press enter\r");
	 fgetc(&__stdin);
   printf("%s", "FatFS's mounting...\r");


  /*##-1- Link the micro SD disk I/O driver ##################################*/
	if(FATFS_LinkDriver(&SD_Driver, SDPath) == 0)
  {
    /*##-2- Register the file system object to the FatFs module ##############*/
    if(f_mount(&SDFatFs, (TCHAR const*)SDPath, 0) != FR_OK)
    {
      /* FatFs Initialization Error */
      Error_Handler();
    }
    else
    {
      /*##-3- Create a FAT file system (format) on the logical drive #########*/
      /* WARNING: Formatting the uSD card will delete all content on the device */
/*                        
      if(f_mkfs((TCHAR const*)SDPath, 0, 0) != FR_OK)
      {
        // FatFs Format Error
        Error_Handler();
      }

      else */
      {
//       	pin_set(ledPORT, led2);

        /*##-4- Create and Open a new text file object with write access #####*/
// 	 f_getcwd(termStr, 90);
				
//				printf("%s%s%s", "current dir: ",termStr," \r");
				
				
   printf("%s", "Enter filename\r");
//	 fgetc(&__stdin);
   while (scanf("%s",fileName) != 1) {} ;
//   printf("%s", "FatFS mounting...\r");
				
				
        if(f_open(&MyFile, fileName, FA_CREATE_ALWAYS | FA_WRITE) != FR_OK) //"_HK.TXT"
        {
          /* 'STM32.TXT' file Open for write Error */
          Error_Handler();
        }
        else
        {
          /*##-5- Write data to the text file ################################*/
					
   printf("%s", "String for write\r");
//	 fgetc(&__stdin);
   while (scanf("%s",wrStr) != 1) {} ;
//   printf("%s", "FatFS mounting...\r");
					
          res = f_write(&MyFile, wrStr, strlen(wrStr), (void *)&byteswritten); // wtext

          /*##-6- Close the open text file #################################*/
          if (f_close(&MyFile) != FR_OK )
          {
            Error_Handler();
          }

          if((byteswritten == 0) || (res != FR_OK))
          {
            /* 'STM32.TXT' file Write or EOF Error */
            Error_Handler();
          }
          else
          {
            /*##-7- Open the text file object with read access ###############*/
            if(f_open(&MyFile, fileName, FA_READ) != FR_OK)
            {
              /* 'STM32.TXT' file Open for read Error */
              Error_Handler();
            }
            else
            {
              /*##-8- Read data from the text file ###########################*/
              res = f_read(&MyFile, wrStr, strlen(wrStr), (UINT*)&bytesread);

              if((bytesread == 0) || (res != FR_OK))
              {
                /* 'STM32.TXT' file Read or EOF Error */
                Error_Handler();
              }
              else
              {
                /*##-9- Close the open text file #############################*/
                f_close(&MyFile);

                /*##-10- Compare read data with the expected data ############*/
                if((bytesread != byteswritten))
                {
                  /* Read data is different from the expected data */
                  Error_Handler();
                }
                else
                {
                  /* Success of the demo: no error occurrence */
//                  BSP_LED_On(LED_GREEN);
									  pin_set(ledPORT, led2);

                }
              }
            }
          }
        }
      }
    }
  }

  /*##-11- Unlink the RAM disk I/O driver ####################################*/
  FATFS_UnLinkDriver(SDPath);
	
	rtsWaitEvent();
}
