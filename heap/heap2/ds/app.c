  ds_init(0); // 0 = pointer to 'READY' handler

  int16_t temp;
  temp = ds_read_temp();

  if ( (temp == DS_NOT_PRESENT) || (temp == DS_CRC_ERROR) ) vis_temp = 0xff;
          else if (temp < 0 ) vis_temp = 0; else {vis_temp = temp/16; if (vis_temp > 99) vis_temp = 99;}
