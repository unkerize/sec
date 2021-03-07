 ;-------------------------------------------
 ; STM32F0XX
 ;-------------------------------------------
 IF :DEF:STM32F030
   INCLUDE rts_cm0.s
   INCLUDE startup\stm32f030.s
 ENDIF
 ;-------------------------------------------
 IF :DEF:STM32F051
   INCLUDE rts_cm0.s
   INCLUDE startup\stm32f051.s
 ENDIF
 ;-------------------------------------------
 ;-------------------------------------------
 INCLUDE drv\chips_10x.s


 IF :DEF:STM32F10X_MD

 INCLUDE rts_cm3.s

 INCLUDE startup\stm32f10x_md.s

 ENDIF
 ;-------------------------------------------
 IF :DEF:STM32F10X_MD_VL

 INCLUDE rts_cm3.s

 INCLUDE startup\stm32f10x_md_vl.s

 ENDIF
 ;-------------------------------------------
 IF :DEF:STM32F303x6 :LOR: :DEF:STM32F303x8

 INCLUDE rts_cm3.s

 INCLUDE startup\stm32f303x8.s

 ENDIF
 ;-------------------------------------------
 IF :DEF:STM32F303xB :LOR: :DEF:STM32F303xC

 INCLUDE rts_cm3.s

 INCLUDE startup\stm32f303xc.s

 ENDIF
 ;-------------------------------------------
 IF :DEF:STM32F334

 INCLUDE rts_cm3.s

 INCLUDE startup\stm32f334.s

 ENDIF
 ;-------------------------------------------
 IF :DEF:STM32G474

 INCLUDE rts_cm3.s

 INCLUDE startup\stm32g474xx.s

 ENDIF
 ;-------------------------------------------
 END



