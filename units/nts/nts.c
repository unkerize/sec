
#include "system.h"
#include "nts.h"
#include "pins.h"
#include <stdint.h>


ntsType* nts[ntsNumber];
uint8_t ntsCounter = 0;

ntsType* ntsGetRxHandle(void)
{
  uint8_t i;
        uint8_t th;

        th = stGetTaskID();

  for (i = 0; i < ntsNumber; i++) if (nts[i]->rxsth == th) return nts[i];
  while (1) {};
}

ntsType* ntsGetTxHandle(void)
{
  uint8_t i;
        uint8_t th;

        th = stGetTaskID();

  for (i = 0; i < ntsNumber; i++) if (nts[i]->txsth == th) return nts[i];
  while (1) {};
}


void ntsAddCm(ntsType* ntsh, uint8_t cm_index, fptr cm_handler)
{
  if (cm_index  > ntsCmNumber-1 ) return;

  ntsh->cmList[cm_index] = cm_handler;
}

void ntsTxHandler(void)
{
  ntsType*  ntsh;

  ntsh = ntsGetTxHandle();

  ntsSendPack(ntsh);
}

void ntsSendPack(ntsType*  ntsh)
{
  uint16_t crc;

  if (ntsh->txSize < 2) return;

  ntsh->txBuffer[0] = ntsh->rxBuffer[0];
  ntsh->txBuffer[1] = ntsh->rxBuffer[1];

  switch(ntsh->crcType)
  {
  case ntsCRC_NON:
    break;
  case ntsCRC8:
    ntsh->txBuffer[ntsh->txSize] = calcCRC8(ntsh->txBuffer, ntsh->txSize);
    ntsh->txSize++;
    break;
  case ntsCRC16:
    crc = calcCRC16(ntsh->txBuffer, ntsh->txSize);

    ntsh->txBuffer[ntsh->txSize+0] = crc & 0xff;
    ntsh->txBuffer[ntsh->txSize+1] = (crc>>8) & 0xff;

    ntsh->txSize += 2;
    break;
  }

  uartTxBlock(ntsh->uarth, ntsh->txBuffer, ntsh->txSize);
}


void ntsRxHandler(void)
{
  ntsType*  ntsh;

  ntsh = ntsGetRxHandle();

  ntsh->rxSize = uartGetRxSize(ntsh->uarth);

  uartResetRx(ntsh->uarth);

  if (ntsh->rxSize == 0) return;

  if (ntsh->rxBuffer[0] != ntsh->Addr) return;

  if (ntsh->rxBuffer[1] > ntsCmNumber-1) return;


  switch(ntsh->crcType)
  {
  case ntsCRC_NON:
    if (ntsh->rxSize < 2) return;
    break;

  case ntsCRC8:
    if (ntsh->rxSize < 3) return;
    if (calcCRC8(ntsh->rxBuffer, ntsh->rxSize--) ) return;
    if (testForZero(ntsh->rxBuffer, ntsh->rxSize) ) return;
    break;

  case ntsCRC16:
    if (ntsh->rxSize < 4) return;
    if (calcCRC16(ntsh->rxBuffer, ntsh->rxSize) ) return;
    ntsh->rxSize -= 2 ;
    break;
  };

  if (ntsh->cmList[ntsh->rxBuffer[1]]) ntsh->cmList[ntsh->rxBuffer[1]]();

  if (ntsh->Ack)
  {
    if (ntsh->sepTx) stSetEvent(ntsh->txsth);
    else ntsSendPack(ntsh);
  }

}

uint8_t ntsAdd(ntsType* ntshl)
{
  uartCfgType uart_cfg;
//  uint32_t ntsOptions;

  nts[ntsCounter] = ntshl;

  ntshl->rxsth = stAddTask(&ntsRxHandler, stf_event);
  ntshl->txsth = stAddTask(&ntsTxHandler, stf_event);

        uart_cfg.Port = ntshl->port;
        uart_cfg.BaudeRate = ntshl->BaudeRate;
        uart_cfg.rxBuf = ntshl->rxBuffer;
        uart_cfg.rxBufSize = ntshl->rxBufSize;
        uart_cfg.Port485 = ntshl->dirPort;
        uart_cfg.Pin485 = ntshl->dirPin;
        uart_cfg.osTxFlag = 0;
        uart_cfg.osTxIndex = 0;
        uart_cfg.osRxFlag = uartOsST;
        uart_cfg.osRxIndex = ntshl->rxsth;

  ntshl->uarth = uartAdd(&uart_cfg);

  return(ntsCounter++);
}
