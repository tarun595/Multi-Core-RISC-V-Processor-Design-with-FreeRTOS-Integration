timer_driver.c

#include "drivers.h"

// Reads the 64-bit memory-mapped mtime register safely
static uint64_t read_mtime(void) {
    volatile uint32_t *mtime_low = (volatile uint32_t *)(CLINT_MTIME);
    volatile uint32_t *mtime_high = (volatile uint32_t *)(CLINT_MTIME + 4);

    uint32_t high, low;
    // To avoid rollover, read high then low then high again.
    do {
        high = *mtime_high;
        low = *mtime_low;
    } while (high != *mtime_high); // If high changed, low may have rolled over, so retry.

    return ((uint64_t)high << 32) | low;
}

// Writes to the 64-bit memory-mapped mtimecmp register for a given hart
static void write_mtimecmp(uint32_t hart_id, uint64_t value) {
    volatile uint32_t *mtimecmp = (volatile uint32_t *)(CLINT_MTIMECMP(hart_id));
    // Disable timer interrupt around update to avoid spurious interrupts
    // This is a simple method: write upper half to a very large value first
    mtimecmp[1] = 0xFFFFFFFF; // Write high word to max value
    mtimecmp[0] = (uint32_t)value; // Write low word
    mtimecmp[1] = (uint32_t)(value >> 32); // Write high word
}

uint64_t Timer_GetTime(void) {
    return read_mtime();
}

void Timer_SetCompare(uint32_t hart_id, uint64_t compare_value) {
    write_mtimecmp(hart_id, compare_value);
}

void Timer_Init(uint32_t hart_id, uint32_t tick_rate_hz) {
    // Calculate the number of clock cycles per tick
    uint64_t ticks_per_interrupt = SYSTEM_FREQ_HZ / tick_rate_hz;

    // Set the first timer interrupt to be in the future
    uint64_t current_time = read_mtime();
    uint64_t first_timeout = current_time + ticks_per_interrupt;

    Timer_SetCompare(hart_id, first_timeout);
}

void Timer_ClearInterrupt(uint32_t hart_id) {
    // Reading the time or writing to mtimecmp often clears the interrupt condition in the CPU.
    // For CLINT, the interrupt is cleared by writing to mtimecmp, which we do in Timer_SetCompare.
    // This function is a placeholder if any other action is needed.
    (void)hart_id;
}
