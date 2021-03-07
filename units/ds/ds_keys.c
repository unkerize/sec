
#define ds_dev_count 0

#undef  DS_USE_CMD
#define DS_USE_CMD  0

#include "ds_cmd.c"

void ds_manager(void)
{
  uint8_t i;

  ds_pin_opendrain();

  while(1)
  {
    memset(ds_scratch, 0xff, 8);

    if (!ds_reset() ) continue;

    ds_write_byte(DS_CMD_READ_ROM);

    for (i = 0; i < 8; i++) ds_scratch[i] = ds_read_byte();

    if (checkCRC8(ds_scratch, 8) ) continue;

    if (ds_pre_handler) ds_pre_handler();

    while (ds_reset() ) {};

    if (ds_post_handler) ds_post_handler();
  }
}

int16_t ds_read_temp(uint8_t l_index)
{
  return DS_NOT_PRESENT;
}
