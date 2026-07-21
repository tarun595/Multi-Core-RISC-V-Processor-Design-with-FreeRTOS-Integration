freertos_handlers.c

#include "freertos_bsp.h"
#include "FreeRTOS.h"
#include "timers.h"

/* --- Global Interrupt Handler (Called from Assembly) --- */
uint32_t ulBSPHandleInterrupt( uint32_t ulCause, uint32_t ulEpc ) {
    uint32_t ulReturnEpc = ulEpc;
    /* Check the interrupt cause (MCAUSE register) */
    switch( ulCause ) {
        case 0x80000007: /* Machine Timer Interrupt */
            /* Clear the timer interrupt (handled in assembly setup) */
            /* Call the FreeRTOS tick handler */
            if( xTaskIncrementTick() != pdFALSE ) {
                /* A task switch is required. Pend the PendSV interrupt. */
                vBSPSendIPI( xPortGetCoreID() ); // Or use a software interrupt source
            }
            /* Program the timer for the next tick */
            vBSPSetupTimerInterrupt( configTICK_RATE_HZ );
        break;

        case 0x8000000B: /* Machine External Interrupt (from PLIC) */
            /* This could be a UART interrupt or an IPI from another core */
            // uint32_t ulIntId = PLIC_CLAIM;
            // ... Handle the specific interrupt ...
            // PLIC_COMPLETE = ulIntId;
        break;

        default:
            /* Handle other exceptions (e.g., illegal instruction) */
            vBSPPutS("Unhandled Exception on Core ");
            vBSPPutC( '0' + xPortGetCoreID() );
            vBSPPutS("\n");
            for( ;; ); // Halt
        break;
    }
    return ulReturnEpc;
}

/* Helper function to get core ID */
uint32_t xPortGetCoreID( void ) {
    uint32_t ulCoreId;
    asm volatile ( "csrr %0, mhartid" : "=r"( ulCoreId ) );
    return ulCoreId;
}
