
                AREA    |.text|, CODE, READONLY

                                EXPORT rts_entry
                                EXPORT rts_exit
                                EXPORT rts_move_regs
                                EXTERN rts_SaveSP
;--------------------------------------------------
rts_entry       PROC
                                ldr     R1, =rts_SaveSP
                                mrs R3,PSR
                                push {R3-R12,LR}
                                str SP, [R1]
                                ldr SP,[R0]

                                pop {R3-R12}
                                msr PSR,R3
                                pop {PC}
                ENDP
;--------------------------------------------------
rts_exit        PROC
                                ldr     R1, =rts_SaveSP
                                mrs R3,PSR
                                push {R3-R12,LR}
                                str SP, [R0]
                                ldr SP,[R1]

                                pop {R3-R12}
                                msr PSR,R3
                                pop {PC}
                ENDP
;--------------------------------------------------
; R0 = pointer to task
; R1 = pointer to end of sstack
rts_move_regs   PROC
                  stmdb R1!,{R0}
                  mrs R3,PSR
                  stmdb R1,{R3-R12}
                  bx LR

                ENDP
;--------------------------------------------------
                END

