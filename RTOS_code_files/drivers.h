Part 2 below

drivers.h



#ifndef DRIVERS_H
#define DRIVERS_H

#include <stdint.h>
#include <stdbool.h>

/* --- Hardware Memory Map --- */
#define UART0_BASE        ( 0x10000000 )
#define CLINT_BASE        ( 0x2000000 )
#define PLIC_BASE         ( 0x0C000000 )
#define SYSTEM_FREQ_HZ    ( 100000000 ) // 100 MHz Simulink clock assumption

/* UART Register Offsets */
#define UART_TX_DATA_OFFSET   ( 0x00 )
#define UART_RX_DATA_OFFSET   ( 0x04 )
#define UART_STATUS_OFFSET    ( 0x08 )
#define UART_CTRL_OFFSET      ( 0x0C )

/* UART Status Register Bits */
#define UART_STATUS_TX_FULL   ( 1 << 0 )
#define UART_STATUS_RX_EMPTY  ( 1 << 1 )
#define UART_STATUS_TX_BUSY   ( 1 << 2 )

/* CLINT Register Offsets */
#define CLINT_MTIME           ( CLINT_BASE + 0xBFF8 )
#define CLINT_MTIMECMP( i )   ( CLINT_BASE + 0x4000 + 8 * ( i ) )
#define CLINT_MSIP( i )       ( CLINT_BASE + 4 * ( i ) ) // IPI Register for Hart i

/* PLIC Registers */
#define PLIC_PRIORITY( int_id )   ( PLIC_BASE + 4 * ( int_id ) )
#define PLIC_PENDING( int_id )    ( PLIC_BASE + 0x1000 + 4 * ( (int_id) / 32 ) )
#define PLIC_ENABLE( hart, int_id ) ( PLIC_BASE + 0x2000 + 0x80 * ( hart ) + 4 * ( (int_id) / 32 ) )
#define PLIC_THRESHOLD( hart )    ( PLIC_BASE + 0x200000 + 0x1000 * ( hart ) )
#define PLIC_CLAIM( hart )        ( PLIC_BASE + 0x200004 + 0x1000 * ( hart ) )

/* Interrupt IDs (Platform Specific) */
#define INT_ID_UART0         ( 1 )
#define INT_ID_IPI           ( 2 ) // Software Interrupt (IPI)

/* --- Driver API --- */

// UART Driver
void UART_Init(void);
void UART_Write(char c);
char UART_Read(char *c);
void UART_WriteString(const char *str);

// System Timer Driver (CLINT)
void Timer_Init(uint32_t hart_id, uint32_t tick_rate_hz);
void Timer_SetCompare(uint32_t hart_id, uint64_t compare_value);
uint64_t Timer_GetTime(void);
void Timer_ClearInterrupt(uint32_t hart_id);

// Inter-Processor Interrupt (IPI) Driver (CLINT)
void IPI_Init(uint32_t hart_id);
void IPI_Send(uint32_t target_hart_id);
void IPI_Clear(uint32_t hart_id);
bool IPI_IsPending(uint32_t hart_id);

// External Interrupt Controller Driver (PLIC)
void PLIC_Init(uint32_t hart_id);
void PLIC_SetPriority(uint32_t int_id, uint8_t priority);
void PLIC_EnableInterrupt(uint32_t hart_id, uint32_t int_id, bool enable);
void PLIC_SetThreshold(uint32_t hart_id, uint8_t threshold);
uint32_t PLIC_ClaimInterrupt(uint32_t hart_id);
void PLIC_CompleteInterrupt(uint32_t hart_id, uint32_t int_id);

#endif /* DRIVERS_H */
