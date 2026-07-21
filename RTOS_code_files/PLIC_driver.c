PLIC_driver.c

#include "drivers.h"

// Helper macro to calculate register offsets for enable and pending arrays
static volatile uint32_t* get_plic_enable_reg(uint32_t hart_id, uint32_t int_id) {
    uint32_t register_index = int_id / 32;
    return (volatile uint32_t*)(PLIC_ENABLE(hart_id, int_id) + 4 * register_index);
}

static uint32_t get_plic_enable_bit(uint32_t int_id) {
    return 1 << (int_id % 32);
}

void PLIC_Init(uint32_t hart_id) {
    // Set default threshold for this hart (accept all interrupts with priority > 0)
    *(volatile uint32_t *)PLIC_THRESHOLD(hart_id) = 0;
    // Ensure UART and IPI interrupts are disabled at the PLIC level initially
    // They will be enabled by the respective driver init functions.
}

void PLIC_SetPriority(uint32_t int_id, uint8_t priority) {
    *(volatile uint32_t *)PLIC_PRIORITY(int_id) = priority;
}

void PLIC_EnableInterrupt(uint32_t hart_id, uint32_t int_id, bool enable) {
    volatile uint32_t *enable_reg = get_plic_enable_reg(hart_id, int_id);
    uint32_t enable_bit = get_plic_enable_bit(int_id);

    if (enable) {
        *enable_reg |= enable_bit;
    } else {
        *enable_reg &= ~enable_bit;
    }
}

void PLIC_SetThreshold(uint32_t hart_id, uint8_t threshold) {
    *(volatile uint32_t *)PLIC_THRESHOLD(hart_id) = threshold;
}

uint32_t PLIC_ClaimInterrupt(uint32_t hart_id) {
    // Reading the claim register returns the ID of the highest-priority pending interrupt
    // and automatically clears its pending bit.
    return *(volatile uint32_t *)PLIC_CLAIM(hart_id);
}

void PLIC_CompleteInterrupt(uint32_t hart_id, uint32_t int_id) {
    // Writing the interrupt ID back to the claim/complete register signals
    // the PLIC that we have finished servicing it.
    *(volatile uint32_t *)PLIC_CLAIM(hart_id) = int_id;
}
