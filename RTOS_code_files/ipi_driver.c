ipi_driver.c


#include "drivers.h"

#define MSIP_REG(hart_id) (*(volatile uint32_t *)(CLINT_MSIP(hart_id)))

void IPI_Init(uint32_t hart_id) {
    // Ensure our own IPI interrupt is cleared at startup
    IPI_Clear(hart_id);
}

void IPI_Send(uint32_t target_hart_id) {
    // Writing any non-zero value to the target hart's MSIP register
    // raises a software interrupt on that core.
    MSIP_REG(target_hart_id) = 0x1;
}

void IPI_Clear(uint32_t hart_id) {
    // Writing zero to our own MSIP register clears the interrupt.
    MSIP_REG(hart_id) = 0x0;
}

bool IPI_IsPending(uint32_t hart_id) {
    // Check if the MSIP bit is set, indicating a pending IPI.
    return (MSIP_REG(hart_id) != 0x0);
}
