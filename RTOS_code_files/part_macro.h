portmacro.h

/* File: portmacro.S */
.equ portCONTEXT_SIZE, ( 32 * 4 ) /* 32 registers (x1-x31, plus maybe mstatus/mepc) */

.macro portSAVE_CONTEXT
    addi sp, sp, -portCONTEXT_SIZE
    sw x1, 1*4(sp)
    sw x2, 2*4(sp)
    /* ... Save all other registers (x3-x31) ... */
    csrr t0, mstatus
    sw t0, 31*4(sp)
    csrr t0, mepc
    sw t0, 32*4(sp)
.endm

.macro portRESTORE_CONTEXT
    lw t0, 32*4(sp)
    csrw mepc, t0
    lw t0, 31*4(sp)
    csrw mstatus, t0
    /* ... Restore all other registers (x3-x31) ... */
    lw x2, 2*4(sp)
    lw x1, 1*4(sp)
    addi sp, sp, portCONTEXT_SIZE
.endm
