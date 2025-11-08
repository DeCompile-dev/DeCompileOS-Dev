#ifndef KEYBOARD_DRIVER_H
#define KEYBOARD_DRIVER_H

#include <stdint.h> // Required for uint8_t

// PS/2 Port Definitions
#define PS2_DATA_PORT 0x60
#define PS2_STATUS_COMMAND_PORT 0x64

// Master PIC Port Definitions
#define PIC1_COMMAND_PORT 0x20
#define PIC1_DATA_PORT 0x21

/**
 * @brief Initializes the keyboard driver.
 * Unmasks IRQ 1 on the PIC controller.
 */
void driver_keyboard_init();

/**
 * @brief Main function for the keyboard interrupt handler (IRQ 1).
 * Reads the scancode, maps it to ASCII, and prints the character.
 * This function is meant to be called from the assembly interrupt stub.
 */
void keyboard_interrupt_handler();

#endif // KEYBOARD_DRIVER_H
