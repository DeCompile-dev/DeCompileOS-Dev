#include "io.h"
#include "print.h"
#include "register.h"
#include "keyboard_driver.h"
#include "memory.h"
#include "pci.h"
#include <stdint.h>
#include "code.h"

#define STARTER_PRINT 0x01
#define PS2_DATA_PORT 0x60
#define PS2_CMD_PORT 0x64

void init_keyboard() {
    inb(PS2_DATA_PORT); 
    
    outb(PS2_CMD_PORT, 0xAD); 
    io_wait(); 
    
    outb(PS2_DATA_PORT, 0xF0); 
}

void kernel_main() {
    uint32_t code_variable = code_variable;
    uint32_t code_result = code_fuction();
    enum video_type
    {
        VIDEO_TYPE_NONE = 0x00,
        VIDEO_TYPE_COLOUR = 0x20,
        VIDEO_TYPE_MONOCHROME = 0x30,
    };

    uint16_t detect_bios_area_hardware(void)
    {
        const uint16_t* bda_detected_hardware_ptr = (const uint16_t*) 0x410;
        return *bda_detected_hardware_ptr;
    }

    enum video_type get_bios_area_video_type(void)
    {   
        return (enum video_type) (detect_bios_area_hardware() & 0x30);
    }
    print_set_color(PRINT_COLOR_YELLOW, PRINT_COLOR_BLACK);   

    init_keyboard();

    driver_keyboard_init();
    print_str("DeCompileOS booting \n");
    print_str("                                                             Build Info: ");
    print_str(__TIME__);
    print_str("\n");
    print_str(">");
}