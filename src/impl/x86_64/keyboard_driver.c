#include "keyboard_driver.h"
#include "print.h"
#include "io.h"

static const char us_scancode_map[] = {
    0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b', 
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', 
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0,      
    '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, 0, 0, ' ',   
};

static void keyboard_flush_buffer() {
    inb(PS2_DATA_PORT); 
}

static void send_eoi() {
    outb(PIC1_COMMAND_PORT, 0x20); 
}


void driver_keyboard_init() {

    uint8_t mask = inb(PIC1_DATA_PORT); 
    
    outb(PIC1_DATA_PORT, mask & ~0x02); 
    
}


void keyboard_interrupt_handler() {

    uint8_t scancode = inb(PS2_DATA_PORT); 
    
    if (scancode & 0x80) {

    } else {
        if (scancode < sizeof(us_scancode_map) / sizeof(us_scancode_map[0])) {
            char ascii_char = us_scancode_map[scancode];
            
            if (ascii_char != 0) {
                char str[2] = {ascii_char, '\0'};
                print_str(str);
                
                if (ascii_char == '\n') {
                    print_str(">");
                }
            }
        }
    }
    send_eoi();
}