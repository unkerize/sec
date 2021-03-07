void ds_manager(void)
{
  uint8_t i;

  ds_error = 0;

  #if DS_USE_RESOLUTION == 1
    ds_set_resolution();
  #endif

  if (ds_error != 0)
  {
    ds_error = DS_RESOLUTION_ERR;

    ds_temp[0] = ds_error;

    rtsSleep();

    while(1) rtsDelay(1000);
  }

  while(1)
  {
    while(1)
    {
      #if DS_USE_CMD == 1
        ds_process_cmd();
      #endif

      if (ds_pre_handler) ds_pre_handler();

      ds_pin_opendrain();
      if (! ds_reset() ) {ds_error = DS_NOT_PRESENT; break;}

      ds_pin_pushpull();

      ds_write_byte(DS_CMD_SKIP_ROM);
      ds_write_byte(DS_CMD_START_CONV);

      #if DS_USE_RESOLUTION == 1
        rtsDelay(ds_conv_time[DS_RESOLUTION]);
      #else
        rtsDelay(760);
      #endif

      ds_pin_opendrain();
      if (! ds_reset() ) {ds_error = DS_NOT_PRESENT; break;}

      ds_pin_pushpull();
      ds_write_byte(DS_CMD_SKIP_ROM);
      ds_write_byte(DS_CMD_READ_SCRATCH_PAD);

      ds_pin_opendrain();
      for (i=0; i<9; i++) ds_scratch[i] = ds_read_byte();
      if (checkCRC8(ds_scratch, 9) ) {ds_error = DS_CRC_ERROR; break; }

      ds_temp[0] = ds_scratch[0] | (ds_scratch[1] << 8);

      ds_error = 0;

      #if DS_FILTER != 0
        ds_filter_counter = 0;
      #endif

      if (ds_post_handler) ds_post_handler();
    }

    ds_error_handler(0);
  }
}

