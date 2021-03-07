
int16_t ds_error;

#if DS_FILTER != 0
  uint8_t ds_filter_counter;
#endif

#if DS_USE_MULTIPOINT == 0
  #define ds_dev_count 1
  int16_t ds_temp[1];
#endif

#if DS_USE_MULTIPOINT == 1
  int8_t   ds_dev_count;
  int8_t   ds_dev_counter;
  uint64_t  ds_addr_buf[DS_DEV_MAX];
  int16_t   ds_temp[DS_DEV_MAX];
#endif

#if DS_USE_RESOLUTION == 1
  const uint16_t ds_conv_time[4] = {95, 190, 380, 760};

void ds_set_resolution(void)
{
  uint8_t i;

  ds_scratch[0] = 0;
  ds_scratch[1] = 0;
  ds_scratch[2] = (DS_RESOLUTION << 5);

  ds_pin_opendrain();
  if (!ds_reset() ) {ds_error = DS_NOT_PRESENT; return;}

  ds_pin_pushpull();
  ds_write_byte(DS_CMD_SKIP_ROM);

  ds_write_byte(DS_CMD_WRITE_SCRATCH_PAD);

  for (i = 0; i < 3; i++) ds_write_byte( ds_scratch[i] );
}
#endif

void ds_error_handler(uint8_t dev)
{
  rtsDelay(10);

#if DS_FILTER == 0
  ds_temp[dev] = ds_error;
  ds_error = 0;
#else
  if (++ds_filter_counter < DS_FILTER) return;

  ds_temp[dev] = ds_error;
  ds_filter_counter = 0;
  ds_error = 0;
#endif
}

#include "ds_cmd.c"

#if DS_USE_MULTIPOINT == 0
  #include "ds_singl.c"
#endif

#if DS_USE_MULTIPOINT == 1
  #include "ds_multi.c"
#endif

int16_t ds_read_temp(uint8_t l_index)
{
  if (l_index > (ds_dev_count-1) ) return DS_NOT_PRESENT;

  return ds_temp[l_index];
}

