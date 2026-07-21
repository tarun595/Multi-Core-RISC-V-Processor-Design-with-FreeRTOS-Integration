interrupt_handler.c


#include "drivers.h"
#include "FreeRTOS.h"
#include "task.h"

// External function to get core ID
extern uint32_t xPortGetCoreID( void );

// The main interrupt handler called from the assembly exception code
uint32_t handle_irq(uint32_t cause, uint32_t epc) {
    uint32_t new_epc = epc;
    uint32_t core_id = xPortGetCoreID();
    int32_t irq_code = cause & 0x7FFFFFFF; // Strip the interrupt flag

    if (cause & 0x80000000) {
        // This is an interrupt
        switch (irq_code) {
            case 3: // Machine Software Interrupt (MSI) - IPI
                if (IPI_IsPending(core_id)) {
                    IPI_Clear(core_id); // Clear the source
                    // An IPI is often used to force a context switch
                    // Pend the PendSV handler if the scheduler says so
                    if( xTaskIncrementTick() != pdFALSE ) {
                        vTaskSwitchContext(); // The actual switch will happen in PendSV
                    }
                }
                break;

            case 7: // Machine Timer Interrupt (MTI)
                Timer_ClearInterrupt(core_id);
                // Handle the FreeRTOS tick
                if( xTaskIncrementTick() != pdFALSE ) {
                    vTaskSwitchContext();
                }
                // Set the next timer interrupt
                Timer_SetCompare(core_id, Timer_GetTime() + (SYSTEM_FREQ_HZ / configTICK_RATE_HZ));
                break;

            case 11: // Machine External Interrupt (MEI) - from PLIC
                {
                    uint32_t int_id = PLIC_ClaimInterrupt(core_id);
                    switch (int_id) {
                        case INT_ID_UART0:
                            // Handle UART interrupt (e.g., received a character)
                            // ... (push char to a ring buffer) ...
                            break;
                        case INT_ID_IPI:
                            // Alternatively, handle IPI through PLIC
                            IPI_Clear(core_id);
                            break;
                        case 0: // Spurious interrupt
                            break;
                        default:
                            UART_WriteString("Unhandled PLIC IRQ: ");
                            // ... print int_id ...
                            break;
                    }
                    PLIC_CompleteInterrupt(core_id, int_id);
                }
                break;

            default:
                UART_WriteString("Unhandled IRQ cause: ");
                // ... print cause ...
                break;
        }
    } else {
        // This is a synchronous exception (e.g., illegal instruction)
        UART_WriteString("Exception on core ");
        // ... print core_id and cause ...
        for(;;); // Halt
    }
    return new_epc;
}
