
#include "system.h"
#include <stdio.h>
#include "term.h"
#include "uart.h"
#include "gpio.h"


struct __FILE { int handle; /* Add whatever you need here */ };
FILE __stdout;
FILE __stdin;

//uint32_t 

#define terminalTxSize (terminalMaxLen+4)
#define terminalRxSize (terminalMaxLen+4)

typedef struct {
  uint8_t txBuf[terminalTxSize];
  uint8_t rxBuf[terminalRxSize];

  uint8_t putBuf[terminalTxSize];
  uint8_t getBuf[terminalRxSize];

        uint8_t put_cnt;
        uint8_t put_in;
        uint8_t put_out;

        uint8_t get_cnt;
        uint8_t get_in;
        uint8_t get_out;
        uint8_t get_len;

        uint8_t uarth;
        uint8_t rtsh;
        uint8_t txsth;
        uint8_t rxsth;
} terminalType;

terminalType terminal;

void terminalTxTask(void)
{
        uint8_t i;

        if (terminal.put_cnt == 0) return;

        if (uartTxBusy(terminal.uarth) == 1 ) return;


  for (i = 0; i < terminal.put_cnt; i++)
        {
    terminal.txBuf[i] = terminal.putBuf[terminal.put_out++];

                if (terminal.put_out == terminalTxSize) terminal.put_out = 0;
        }
        uartTxBlock(terminal.uarth, terminal.txBuf, terminal.put_cnt);

        terminal.put_cnt = 0;
}

void terminalRxTask(void)
{
        uint16_t rxPackSize;
        uint8_t i;

  rxPackSize = uartGetRxSize(terminal.uarth);
  uartResetRx(terminal.uarth);

        if (rxPackSize == 0) return;

        if ( (rxPackSize + terminal.get_cnt) > terminalRxSize ) rxPackSize = terminalRxSize - terminal.get_cnt;

        for (i = 0; i < rxPackSize; i++)
        {
          terminal.getBuf[terminal.get_in++] = terminal.rxBuf[i];

                if (terminal.get_in == terminalRxSize) terminal.get_in = 0;

                terminal.get_cnt++;
        }

        if (rxPackSize != 0) rtsSetEvent(terminal.rtsh);
}



void terminalInit(uint8_t lrtsh)
{
  uartCfgType uart_cfg;
	
	uart_cfg.Port = terminalPORT;
	uart_cfg.BaudeRate = terminalBaudeRate;
	uart_cfg.rxBuf = terminal.rxBuf;
	uart_cfg.rxBufSize = terminalRxSize;
	uart_cfg.Port485 = terminal485port;
	uart_cfg.Pin485 = terminal485pin;
	uart_cfg.osTxFlag = uartOsST;
	uart_cfg.osTxIndex = terminal.txsth;
	uart_cfg.osRxFlag = uartOsST;
	uart_cfg.osRxIndex = terminal.rxsth;
	
        terminal.txsth = stAddTask(&terminalTxTask,stf_event);
        terminal.rxsth = stAddTask(&terminalRxTask,stf_event);
        terminal.rtsh  = lrtsh;

        terminal.put_cnt = 0;
        terminal.put_in  = 0;
        terminal.put_out = 0;

        terminal.get_cnt = 0;
        terminal.get_in  = 0;
        terminal.get_out = 0;
        terminal.get_len = 0;

  terminal.uarth = uartAdd(&uart_cfg);
}

int fputc(int ch, FILE *f)
{
        if (terminal.put_cnt == terminalTxSize) return (ch);

        terminal.putBuf[terminal.put_in++] = (uint8_t) ch;

        if (terminal.put_in == terminalTxSize) terminal.put_in = 0;

        terminal.put_cnt++;

        if (uartTxBusy(terminal.uarth) == 0 ) terminalTxTask();

  return (ch);
}

int fgetc(FILE *f) {
        int retv;

        if (terminal.get_cnt == 0) rtsWaitEvent();

        retv = terminal.getBuf[terminal.get_out++];

        if (terminal.get_out == terminalRxSize) terminal.get_out = 0;

        terminal.get_cnt--;

        terminal.get_len++;

        if (terminal.get_len == terminalMaxLen) retv = 10 ;

        if (retv == 10) terminal.get_len = 0;

        return retv;
}

uint8_t fkbpress(void)
{
  return (terminal.get_cnt != 0);
}
