freertos_bsp.C

#include "freertos_bsp.h"
#include "FreeRTOS.h"
#include "task.h"

/* --- Core-Specific Data --- */
CoreData_t xCoreData[ configNUM_CORES ];

/* --- Initialization --- */
void vBSPInit( uint32_t ulCoreId ) {
    /* Store the core ID */
    xCoreData[ ulCoreId ].ulCoreId = ulCoreId;

    /* Setup the interrupt vector table address.
       The assembly handler for all exceptions/traps. */
    extern void vPortMachineExceptionHandler( void );
    csrw mtvec, vPortMachineExceptionHandler;

    /* Initialize the UART for debugging (Core 0 only) */
    if( ulCoreId == 0 ) {
        /* UART initialization code (set baud rate, etc.) would go here */
    }

    /* Enable machine-mode external interrupts and timer interrupts */
    csrsi mie, 0x888; // MIE_MTIE | MIE_MEIE | MIE_MSIE
    vBSPEnableInterrupts();
}

void vBSPEnableInterrupts( void ) {
    csrsi mstatus, 0x8; // Set MSTATUS_MIE
}

/* --- UART Driver (Polling) --- */
void vBSPPutC( char c ) {
    volatile uint32_t *uart_tx = (uint32_t*)(UART0_BASE + UART_TX_DATA_OFFSET);
    volatile uint32_t *uart_status = (uint32_t*)(UART0_BASE + UART_STATUS_OFFSET);

    /* Wait for UART to be ready to transmit */
    while ( *uart_status & 0x08 ); // Check TX FIFO full bit

    /* Send the character */
    *uart_tx = (uint32_t)c;
}

void vBSPPutS( const char *s ) {
    while( *s ) {
        vBSPPutC( *s++ );
    }
}

/* --- Multi-core Startup --- */
void vBSPWakeOtherCores( void ) {
    /* For a simple implementation, other cores might be spinning in a loop
       in their own startup code, waiting for an IPI. This function sends that IPI. */
    for( int i = 1; i < configNUM_CORES; i++ ) {
        vBSPSendIPI( i );
    }
}

void vBSPSendIPI( uint32_t ulTargetCore ) {
    /* Write to the CLINT's MSIP register for the target core to cause a software interrupt */
    volatile uint32_t *pMSIP = (uint32_t*)(CLINT_BASE + 4 * ulTargetCore);
    *pMSIP = 0x1; // Set the MSIP bit, triggering the interrupt
    // The interrupt handler on the target core will clear this bit.
}
