portasm.h

/* File: portasm.S */
.section .text
.global vPortStartFirstTask
.global xPortPendSVHandler
.global vBSPSetupTimerInterrupt

.include "portmacro.S" /* Includes definitions like portCONTEXT_SIZE */

/* --- vPortStartFirstTask: Starts the scheduler by loading the first task's context --- */
.func vPortStartFirstTask
vPortStartFirstTask:
    /* Disable global interrupts */
    csrrci x0, mstatus, 0x8

    /* Get the address of the TCB of the first task for this core.
       This is defined in the C code (e.g., pxCurrentTCBs[core_id]) */
    la t0, xCoreData
    csrr a0, mhartid          /* Get hardware core ID */
    slli a0, a0, 2            /* Calculate array offset (assuming 4-byte pointers) */
    add t0, t0, a0
    lw sp, (t0)               /* Load the address of the top of the stack from the TCB */
    lw sp, (sp)               /* The first member of the TCB is the pxTopOfStack */

    /* Pop the registers from the stack frame */
    portRESTORE_CONTEXT

    /* Return from exception (mret) will jump to the task's code */
    mret
.endfunc

/* --- xPortPendSVHandler: The PendSV interrupt handler for context switching --- */
.func xPortPendSVHandler
xPortPendSVHandler:
    /* This is an exception handler. The stack pointer (sp) points to a context frame.
       The exception frame (mcause, mepc, etc.) is already on the stack. */

    /* Save the current context (registers) of the task that is being switched out */
    portSAVE_CONTEXT

    /* Store the current stack pointer into the first member of its TCB.
       a0 is used as a scratch register here. */
    la t0, xCoreData
    csrr a0, mhartid
    slli a0, a0, 2
    add t0, t0, a0
    lw a0, 0(t0)      /* Load the address of the current TCB into a0 */
    sw sp, 0(a0)      /* Save the updated SP into the TCB's pxTopOfStack */

    /* Call the FreeRTOS scheduler to find the next task to run.
       This is a C function: vTaskSwitchContext(); */
    call vTaskSwitchContext

    /* Now load the new TCB and its stack pointer. */
    la t0, xCoreData
    csrr a0, mhartid
    slli a0, a0, 2
    add t0, t0, a0
    lw a0, 0(t0)      /* Load the address of the new current TCB into a0 */
    lw sp, 0(a0)      /* Load the new stack pointer from the TCB */

    /* Restore the context of the new task from its stack */
    portRESTORE_CONTEXT

    /* Return from exception, jumping into the new task */
    mret
.endfunc

/* --- vBSPSetupTimerInterrupt: Programs the timer for the next tick --- */
.func vBSPSetupTimerInterrupt
vBSPSetupTimerInterrupt:
    /* a0 contains the value for the next tick (mtime + ulTicks) */
    csrr a1, mhartid
    li t0, CLINT_BASE
    /* Calculate address of this core's mtimecmp register */
    li t1, 8
    mul a1, a1, t1
    add t0, t0, a1
    addi t0, t0, 0x4000

    /* Read current mtime */
    la t2, CLINT_MTIME
    lw t3, 0(t2)
    lw t4, 4(t2)

    /* Add the tick count to the current time */
    add a0, t3, a0
    sw a0, 0(t0)        /* Write low word of mtimecmp */
    /* Handle carry for the high word (simplified) */
    sltu t1, a0, t3
    add t4, t4, t1
    sw t4, 4(t0)        /* Write high word of mtimecmp */

    ret
.endfunc
