
#if DS_USE_CMD == 1
  uint8_t  ds_insert_command;
  uint8_t* ds_cmd_buf;
  int8_t   ds_cmd_indx;
#endif

#if DS_USE_CMD == 0
  #define ds_insert_command 0
#endif

#if DS_USE_CMD == 1

// return 1 - no err
//        0 -    err
uint8_t ds_select_dev(void)
{

#if DS_USE_MULTIPOINT == 1
        uint8_t  i;
        uint8_t* addr;
#endif

  ds_pin_opendrain();

  if (! ds_reset() )
  {
    memset(ds_cmd_buf, 0xff, 9);
    ds_insert_command = 0;
    return 0;
  }

  ds_pin_pushpull();

#if DS_USE_MULTIPOINT == 0

  ds_write_byte(DS_CMD_SKIP_ROM);

  return 1;
#endif

#if DS_USE_MULTIPOINT == 1

  addr = (uint8_t*) &ds_addr_buf[ds_cmd_indx];
  ds_write_byte(DS_CMD_MATCH_ROM);
  for (i = 0; i < 8; i++) ds_write_byte( addr[i] );

  return 1;
#endif
}

void ds_process_cmd(void)
{
  uint8_t i;

  if (ds_insert_command == 0) return;

  if (ds_insert_command == DS_CMD_READ_ROM)
  {
    #if DS_USE_MULTIPOINT == 0
      ds_pin_opendrain();

      if (! ds_reset() )
      {
        memset(ds_cmd_buf, 0xff, 8);
        ds_insert_command = 0;
        return;
      }

      ds_pin_pushpull();

      ds_write_byte(DS_CMD_READ_ROM);

      ds_pin_opendrain();

      for (i = 0; i < 8; i++) ds_cmd_buf[i] = ds_read_byte();

      ds_insert_command = 0;

      if ( checkCRC8(ds_cmd_buf, 8) )
      {
        memset(ds_cmd_buf, 0xff, 8);
        return;
      }
        #endif

    return;
  }

  if (ds_insert_command == DS_CMD_READ_SCRATCH_PAD)
  {

    if (!ds_select_dev()) return;

    ds_write_byte(DS_CMD_READ_SCRATCH_PAD);

    ds_pin_opendrain();

    for (i = 0; i < 9; i++) ds_cmd_buf[i] = ds_read_byte();

    ds_insert_command = 0;

    if ( checkCRC8(ds_cmd_buf, 9) )
    {
      memset(ds_cmd_buf, 0xff, 9);
      return;
    }
    return;
  }

  if (ds_insert_command == DS_CMD_READ_POWER_STATUS)
  {
    if (!ds_select_dev()) return;

    ds_write_byte(DS_CMD_READ_POWER_STATUS);

    ds_pin_opendrain();

    ds_cmd_buf[0] = ds_read_bit();

    ds_insert_command = 0;

    return;
  }
}


// API funcs
void ds_get_addr(uint8_t l_index, uint8_t* buf)
{
  if ( (l_index > (ds_dev_count-1) ) || (ds_insert_command != 0) )
  {
    memset(buf, 0xff, 8);
    return ;
  }

#if DS_USE_MULTIPOINT == 1
  memcpy(buf,&ds_addr_buf[l_index], 8);
#endif

#if DS_USE_MULTIPOINT == 0
  ds_cmd_buf = buf;
  ds_insert_command = DS_CMD_READ_ROM;
#endif
}

void  ds_get_scratchpad(uint8_t l_index, uint8_t* buf)
{
  if ( (l_index > (ds_dev_count-1) ) || (ds_insert_command != 0) )
  {
    memset(buf, 0xff, 9);
    return ;
  }

  ds_cmd_indx = l_index;
  ds_cmd_buf  = buf;
  ds_insert_command = DS_CMD_READ_SCRATCH_PAD;
}

void ds_get_power_status(uint8_t l_index, uint8_t* buf)
{
  if ( (l_index > (ds_dev_count-1) ) || (ds_insert_command != 0) )
  {
    memset(buf, 0xff, 9);
    return ;
  }

  ds_cmd_indx = l_index;
  ds_cmd_buf  = buf;
  ds_insert_command = DS_CMD_READ_POWER_STATUS;
}

#endif

#if DS_USE_CMD == 0

void  ds_get_addr(uint8_t l_index, uint8_t* buf)
{
  memset(buf, 0xff, 9);
}

void  ds_get_scratchpad(uint8_t l_index, uint8_t* buf)
{
  memset(buf, 0xff, 9);
}

void ds_get_power_status(uint8_t l_index, uint8_t* buf)
{
  memset(buf, 0xff, 9);
}

#endif

uint8_t ds_cmd_ready(void)
{
  return ds_insert_command == 0;
}

