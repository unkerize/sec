
#include "ds_addr.c"

void ds_manager(void)
{
  uint8_t i;
  uint8_t* addr;

  ds_search_cycle();

  if (ds_dev_count == 0)
  {
    for (i = 0; i < DS_DEV_MAX; i++) ds_temp[i] = DS_NOT_PRESENT;

    rtsSleep();

    while(1) rtsDelay(1000);
  }

  #if DS_USE_RESOLUTION == 1
    ds_set_resolution();
  #endif

  if (ds_error != 0)
  {
    for (i = 0; i < DS_DEV_MAX; i++) ds_temp[i] = DS_NOT_PRESENT;

    rtsSleep();

    while(1) rtsDelay(1000);
  }

  ds_dev_counter = 0;

  while(1)
  {
    if (ds_pre_handler) ds_pre_handler();

    ds_pin_opendrain();

    ds_reset();

    ds_pin_pushpull();

    ds_write_byte(DS_CMD_SKIP_ROM);
    ds_write_byte(DS_CMD_START_CONV);

    #if DS_USE_RESOLUTION == 1
      rtsDelay(ds_conv_time[DS_RESOLUTION]);
    #else
      rtsDelay(760);
    #endif

    do {

      addr = (uint8_t*) &ds_addr_buf[ds_dev_counter];

      ds_error = 0;

      do{
        do {
          #if DS_USE_CMD == 1
            ds_process_cmd();
          #endif

          ds_pin_opendrain();

          if (!ds_reset() ) {ds_error = DS_NOT_PRESENT; break; }

          ds_pin_pushpull();

          ds_write_byte(DS_CMD_MATCH_ROM);

          for (i = 0; i < 8; i++) ds_write_byte( addr[i] );

          ds_write_byte(DS_CMD_READ_SCRATCH_PAD);

          ds_pin_opendrain();

          for (i = 0; i < 9; i++) ds_scratch[i] = ds_read_byte();

          if (checkCRC8(ds_scratch, 9) ) {ds_error = DS_CRC_ERROR; break; }

          ds_temp[ds_dev_counter] = ds_scratch[0] | (ds_scratch[1] << 8);

          ds_error = 0;

          #if DS_FILTER != 0
            ds_filter_counter = 0;
          #endif
          
        } while(0);

        if (ds_error) ds_error_handler(ds_dev_counter);

      }while (ds_error);

    } while (++ds_dev_counter < ds_dev_count);

    ds_dev_counter = 0;

    if (ds_post_handler) ds_post_handler();
  }
}
