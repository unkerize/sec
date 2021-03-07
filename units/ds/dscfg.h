
// ds TYPE
// 0x0 - teperature cycle
// 0x1 - keys cycle
#define DS_CFG_TYPE            0x1

// 1..127
#define DS_DEV_MAX             0x4

// 0 = dont use filter
// 1..255 - filter deep
#define DS_FILTER              4


#define DS_USE_MULTIPOINT      1
#define DS_USE_RESOLUTION      1
#define DS_USE_CMD             1

// sensor resolution
// 0x0 : 9 bit
// 0x1 : 10bit
// 0x2 : 11bit
// 0x3 : 12bit
#define DS_RESOLUTION 0x1
