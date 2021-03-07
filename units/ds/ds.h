
#ifndef _HK_DS
#define _HK_DS

#define DS_NOT_PRESENT      (-200*16)
#define DS_CRC_ERROR        (-201*16)
#define DS_RESOLUTION_ERR   (-202*16)

#define DS_MHZ 1000000

#define DS_RESET_PULSE      500
#define DS_RESET_ACK_PULSE  550
#define DS_WR0_PULSE    90
#define DS_WR1_PULSE    8
#define DS_RD_PULSE     6
#define DS_RD_ACK_PULSE     (DS_RD_PULSE+5)
#define DS_TIME_SLOT    400

#define DS_CMD_READ_POWER_STATUS  0xB4
#define DS_CMD_READ_SCRATCH_PAD   0xBE
#define DS_CMD_WRITE_SCRATCH_PAD  0x4E
#define DS_CMD_COPY_SCRATCH_PAD   0x48
#define DS_CMD_RECALL_EEPROM      0xB8
#define DS_CMD_START_CONV         0x44

#define DS_CMD_SKIP_ROM           0xCC
#define DS_CMD_SEARCH_ROM         0xF0
#define DS_CMD_READ_ROM           0x33
#define DS_CMD_MATCH_ROM          0x55
#define DS_CMD_ALARM_SEARCH       0xEC

#define DS_FAMILY_1990            0x01
#define DS_FAMILY_18B20           0x28

#define DS_TYPE_TEMP              0x00
#define DS_TYPE_KEYS              0x01

typedef struct{
  TIM_TypeDef*   tim;
  uint8_t        chn;

  GPIO_TypeDef*  port;
  uint8_t        pin;
  uint8_t        alterf;

  fptr pre_handler;
  fptr post_handler;
} ds_cfg_td;

// funcs
fptr ds_init(ds_cfg_td* cfg);

int8_t  ds_count(void);
uint8_t ds_get_type(void);
uint8_t ds_get_multi(void);
uint8_t ds_get_search_status(void);

int16_t  ds_read_temp(uint8_t l_index);
uint64_t* ds_read_key(void);

// cmd
uint8_t ds_cmd_ready(void);
void  ds_get_addr(uint8_t l_index, uint8_t* buf);
void  ds_get_scratchpad(uint8_t l_index, uint8_t* buf);
void ds_get_power_status(uint8_t l_index, uint8_t* buf);


#endif //_HK_DS
