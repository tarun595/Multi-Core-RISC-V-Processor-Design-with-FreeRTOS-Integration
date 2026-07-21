uart_driver.c


#include "drivers.h"

#define UART_REG(offset) (*(volatile uint32_t *)(UART0_BASE + (offset)))

void UART_Init(void) {
    // Simulink UART might be pre-configured. This is a placeholder.
    // In a real hardware, you would set baud rate divisors here.
    // For Simulink, we often assume the model is pre-configured correctly.
}

void UART_Write(char c) {
    // Wait until the Transmit FIFO is not full
    while (UART_REG(UART_STATUS_OFFSET) & UART_STATUS_TX_FULL) {
        // Spin wait
    }
    // Write the character to the TX data register
    UART_REG(UART_TX_DATA_OFFSET) = (uint32_t)c;
}

// Attempts to read a character. Returns true if successful, false if no data.
char UART_Read(char *c) {
    // Check if the Receive FIFO is empty
    if (UART_REG(UART_STATUS_OFFSET) & UART_STATUS_RX_EMPTY) {
        return false;
    }
    // Read the character from the RX data register
    *c = (char)UART_REG(UART_RX_DATA_OFFSET);
    return true;
}

void UART_WriteString(const char *str) {
    while (*str) {
        UART_Write(*str++);
    }
}
