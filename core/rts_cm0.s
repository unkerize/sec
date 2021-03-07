
                AREA    |.text|, CODE, READONLY

                                EXPORT rts_entry
                                EXPORT rts_exit
                                EXPORT rts_move_regs
                                EXTERN rts_SaveSP
;--------------------------------------------------
rts_entry       PROC
                                ldr     R1, =rts_SaveSP
                                mrs R3,PSR

                                push {LR}
								push {R3-R7}

                                mov R3,R8
                                mov R4,R9
                                mov R5,R10
                                mov R6,R11
                                mov R7,R12

                                push {R3-R7}

                                mov R3,SP
                                str R3,[R1]

                                ldr R3,[R0]
                                mov SP,R3

                                pop {R3-R7}

                                mov R8,R3
                                mov R9,R4
                                mov R10,R5
                                mov R11,R6
                                mov R12,R7

                                pop {R3-R7}


                                msr PSR,R3
                                pop {PC}
                ENDP
;--------------------------------------------------
rts_exit        PROC
                                ldr     R1, =rts_SaveSP
                                mrs R3,PSR

                                push {LR}
                                push {R3-R7}

                                mov R3,R8
                                mov R4,R9
                                mov R5,R10
                                mov R6,R11
                                mov R7,R12

                                push {R3-R7}

                                mov R3,SP
                                str R3,[R0]

                                ldr R3,[R1]
                                mov SP,R3

                                pop {R3-R7}

                                mov R8,R3
                                mov R9,R4
                                mov R10,R5
                                mov R11,R6
                                mov R12,R7

                                pop {R3-R7}

                                msr PSR,R3
                                pop {PC}
                ENDP
;--------------------------------------------------
; R0 = pointer to task
; R1 = pointer to end of sstack
rts_move_regs   PROC
                  subs R1,R1,#4
                  str R0,[R1]
				  
                  mrs R3,PSR
				  
                  subs R1,R1,#4
				  str R3,[R1]

                  bx LR

                ENDP
;--------------------------------------------------
                END

