
#pragma pack(push,1)
typedef struct {
  ADC_TypeDef* port;
  uint8_t*     chls;
  uint16_t*    buf;
  uint8_t      chn;

  uint8_t   flt_deep;
  uint8_t   flt_deci;

  uint8_t   os_indx;
  fptr      handler;

  uint16_t  flt_cnt;
  uint8_t   ch_cnt;
  uint32_t  sum;

} adc_type;
#pragma pack(pop)
