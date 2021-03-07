
#include "system.h"
#include "spi.h"
#include "kbkey.h"
#include "pins.h"
#include "mface.h"
#include "mface_cfg.h"

static uint8_t mf_spi_inst;
static uint8_t mf_spi_tx_buf[MF_SPI_SIZE];
static uint8_t mf_spi_rx_buf[MF_SPI_SIZE];

static uint8_t  mf_col_counter   = 0;
static uint8_t  mf_blink_flag    = 0;
static uint16_t mf_blink_counter = 0;


static kbp_t  kbp;
static din_t* mf_din;

static mf_t mf_vis;

static const uint8_t mf_leds_mask[MF_LED_NUM] = {MF_LED_1, MF_LED_2, MF_LED_3, MF_LED_4};
static const uint8_t mf_cols_mask[MF_COL_NUM] = {MF_COL_1, MF_COL_2, MF_COL_3, MF_COL_4};

static volatile uint32_t mf_keys;

static const din_ports_t mf_key_ports[MF_KEY_NUM] = {
 {&mf_keys, MF_KEY_1, 0},
 {&mf_keys, MF_KEY_2, 0},
 {&mf_keys, MF_KEY_3, 0},
 {&mf_keys, MF_KEY_4, 0},
 {&mf_keys, MF_KEY_5, 0},
 {&mf_keys, MF_KEY_6, 0},
 {&mf_keys, MF_KEY_7, 0} };


void mf_main(void)
{
  uint8_t       segs[4];
  uint8_t       leds,i,si;

  pin_set(MF_SPI_LTC_PORT, MF_SPI_LTC_PIN);

  leds = 0;

  for (i = 0; i < MF_LED_NUM; i++)
    if (mf_vis.leds[i] == 2) leds |= mf_blink_flag * mf_leds_mask[i];
      else leds |= (1-mf_vis.leds[i]) * mf_leds_mask[i];

  for (i = 0; i < MF_RAW_NUM; i++)
  {
    si =  i * MF_COL_NUM + mf_col_counter;

    if (mf_vis.segs_blink[si] == 1) segs[i] = mf_blink_flag * mf_vis.segs[si];
      else segs[i] = mf_vis.segs[si];
  }

  mf_spi_tx_buf[0] = mf_cols_mask[mf_col_counter] | leds;
  mf_spi_tx_buf[1] = segs[2];
  mf_spi_tx_buf[2] = segs[1];
  mf_spi_tx_buf[3] = segs[0];
  mf_spi_tx_buf[4] = segs[3];

  pin_clr(MF_SPI_LTC_PORT, MF_SPI_LTC_PIN);

  mf_keys = mf_spi_rx_buf[0];

  for (i = 0; i < MF_KEY_NUM; i++) mf_vis.keys[i] = mf_din[i].state;

  spiTxBlock(mf_spi_inst, mf_spi_tx_buf, mf_spi_rx_buf, MF_SPI_SIZE);

  if (++mf_col_counter == MF_COL_NUM) mf_col_counter = 0;

  if (++mf_blink_counter == MF_BLINK_CONST) {mf_blink_counter = 0; mf_blink_flag = 1 - mf_blink_flag; }
}

mf_t* mf_init(kb_cfg_t* l_keys_cfg)
{
  mf_spi_inst =  spiAdd(MF_SPI_PORT, spioMODE0 |  spioBR8 | spioMSBF | spioOsNON );

  kbp.din_port = (din_ports_t*) mf_key_ports;
  kbp.din_size = MF_KEY_NUM;
  kbp.din_const = 8;
  kbp.din_os_flag = stf_1kHz ;

  kbp.kb_cfg = l_keys_cfg;
  kbp.kb_size = MF_KEY_NUM;

  kbp.kb_click_const = MF_KB_CLICK_CONST;
  kbp.kb_hold_const = MF_KB_HOLD_CONST;
  kbp.kb_os_flag = stf_100Hz;

  mf_din = kb_init(&kbp);

  stAddTask(&mf_main, stf_1kHz);

  return &mf_vis;
}

