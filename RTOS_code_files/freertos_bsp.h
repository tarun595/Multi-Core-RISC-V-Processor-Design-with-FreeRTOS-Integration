freertos_bsp.h

#ifndef FREERTOS_BSP_H
#define FREERTOS_BSP_H

#include <stdint.h>

/* --- Hardware Memory Map --- */
#define UART0_BASE        ( 0x10000000 )
#define CLINT_BASE        ( 0x2000000 )
#define PLIC_BASE         ( 0x0C000000 )

/* UART Register Offsets */
#define UART_TX_DATA_OFFSET ( 0x00 )
#define UART_STATUS_OFFSET  ( 0x08 )

/* CLINT Register Offsets (for Core i) */
#define CLINT_MTIME         ( CLINT_BASE + 0xBFF8 )
#define CLINT_MTIMECMP( i ) ( CLINT_BASE + 0x4000 + 8 * ( i ) )

/* PLIC Registers (simplified) */
#define PLIC_SOURCE_PRIORITY( int_id ) ( PLIC_BASE + 0x0 + 4 * ( int_id ) )
#define PLIC_PENDING_ARRAY             ( PLIC_BASE + 0x1000 )
#define PLIC_ENABLE_ARRAY( hart_id )   ( PLIC_BASE + 0x2000 + 0x80 * ( hart_id ) )
#define PLIC_THRESHOLD_CLAIM( hart_id ) ( PLIC_BASE + 0x200000 + 0x1000 * ( hart_id ) )

/* --- Core-Specific Data Structure --- */
typedef struct {
    volatile uint32_t *pxCurrentTCB;      // Points to the TCB of the currently running task
    uint32_t ulCoreId;                    // Hardware ID of this core (0, 1, ... N-1)
    // Add other core-specific vars here (e.g., nested interrupt count)
} CoreData_t;

extern CoreData_t xCoreData[ configNUM_CORES ];

/* --- BSP Function Prototypes --- */
// Initialization
void vBSPInit( uint32_t ulCoreId );
void vBSPInitClock( void );

// Context Switching
void vPortStartFirstTask( void ) __attribute__((naked));
void xPortSwitchContext( void *pxOldTCB, void *pxNewTCB );

// Interrupt Handling
void vBSPEnableInterrupts( void );
uint32_t ulBSPHandleInterrupt( uint32_t ulCause, uint32_t ulEpc );
void vBSPSetupTimerInterrupt( uint32_t ulTicks );
void vBSPSendIPI( uint32_t ulTargetCore );

// Debug UART
void vBSPPutC( char c );
void vBSPPutS( const char *s );

// Multi-core Startup
void vBSPWakeOtherCores( void );

#endif /* FREERTOS_BSP_H */
