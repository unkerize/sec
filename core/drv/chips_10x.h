
// Low Density
// f101, f102, f103 flash: 16k,32k
#if defined (STM32F101x6) || defined (STM32F102x6) || defined(STM32F103x6)
//  #define STM32F10X_LD
#endif

// f100 flash: 16k,32k
#if defined (STM32F100x6)
  #define STM32F10X_LD_VL
#endif

// Medium Density
// f101, f102, f103 flash: 64k,128k
#if defined (STM32F101xB) || defined (STM32F102xB) || defined(STM32F103xB)
  #define STM32F10X_MD
#endif

// f100 flash: 64k,128k
#if defined (STM32F100xB)
  #define STM32F10X_MD_VL
#endif

// High Density
// f101, f102, f103 flash: 256k,328k, 512k
#if defined (STM32F101xE) || defined (STM32F102xE) || defined(STM32F103xE)
  #define STM32F10X_HD
#endif

// f100 flash: 256k,328k, 512k
#if defined (STM32F100xE)
  #define STM32F10X_HD_VL
#endif

// XL Density
// f101, f102, f103 flash: 768k,1024k
#if defined (STM32F101xG) || defined (STM32F102xG) || defined(STM32F103xG)
  #define STM32F10X_XL
#endif

// f100 flash: 768k,1024k
#if defined (STM32F100xG)
  #define STM32F10X_XL_VL
#endif
