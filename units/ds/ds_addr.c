void ds_search_cycle(void)
{
  uint8_t last_discrepancy;
  uint8_t discrepancy_marker;
  uint8_t done;
  uint8_t bit_a;
  uint8_t bit_b;
  uint8_t rom_bit_index;
  uint64_t rom_bit;
  uint8_t bit_save;

  ds_search_done = 0;

  ds_pin_opendrain();

  bit_save = 1;
  rom_bit = 0xffffffffffffffff;

  ds_dev_count = 0;

  if ( ds_addr_buf == 0 ) sysErrHandler(ser_heap_err);

  last_discrepancy = 0;
  done = 0;

  while ( (done == 0) && (ds_dev_count < DS_DEV_MAX))
  {
    if (! ds_reset() ) return;

    rom_bit_index = 1;
    discrepancy_marker = 0;

    ds_write_byte(DS_CMD_SEARCH_ROM);

    do
    {
      bit_save = rom_bit & 1;
      rom_bit >>= 1;

      bit_a = ds_read_bit();
      bit_b = ds_read_bit();

      if ( bit_a && bit_b ) return;

      if ( (bit_a || bit_b) == 0  )
      {

        if (rom_bit_index == last_discrepancy) {rom_bit |= 0x8000000000000000;}

        else
         {
           if (rom_bit_index > last_discrepancy)
           {
             rom_bit &= ~0x8000000000000000;
             discrepancy_marker = rom_bit_index;
           }
           else
           {
             if ( (bit_save )  & 1) rom_bit |= 0x8000000000000000; else rom_bit &=  ~0x8000000000000000;
             if ( ( (bit_save) & 1) == 0 ) discrepancy_marker = rom_bit_index;
           }
         }
      }
      else
      {
        if (bit_a  & 1) rom_bit |= 0x8000000000000000; else rom_bit &=  ~0x8000000000000000;
      }

      ds_write_bit( rom_bit >> 63 );

      rom_bit_index++;

    } while(rom_bit_index <= 64);

  last_discrepancy = discrepancy_marker;

  if (last_discrepancy == 0) done = 1;

  ds_addr_buf[ds_dev_count] = rom_bit;

  ds_dev_count++;
  }

  ds_search_done = 1;
}
