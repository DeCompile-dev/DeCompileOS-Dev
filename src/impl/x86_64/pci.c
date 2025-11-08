#include "pci.h"
#include "io.h"
#include "memory.h"

#define PCI_CONFIG_ADDRESS 0xCF8
#define PCI_CONFIG_DATA 0xCFC
#define PCI_CLASS_MASS_STORAGE 0x01
#define PCI_SUBCLASS_NVME 0x08
#define PCI_COMMAND_REGISTER 0x04
#define PCI_BAR0 0x10

uint32_t pci_config_read(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset) {
    uint32_t address;
    uint32_t lbus = (uint32_t)bus;
    uint32_t lslot = (uint32_t)slot;
    uint32_t lfunc = (uint32_t)func;
    
    address = (uint32_t)((lbus << 16) | (lslot << 11) | (lfunc << 8) | (offset & 0xFC) | ((uint32_t)0x80000000));
    
    outl(PCI_CONFIG_ADDRESS, address);
    return inl(PCI_CONFIG_DATA);
}

void pci_config_write(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset, uint32_t value) {
    uint32_t address;
    uint32_t lbus = (uint32_t)bus;
    uint32_t lslot = (uint32_t)slot;
    uint32_t lfunc = (uint32_t)func;
    
    address = (uint32_t)((lbus << 16) | (lslot << 11) | (lfunc << 8) | (offset & 0xFC) | ((uint32_t)0x80000000));
    
    outl(PCI_CONFIG_ADDRESS, address);
    outl(PCI_CONFIG_DATA, value);
}

static uint64_t nvme_get_bar(uint8_t bus, uint8_t slot, uint8_t func) {
    uint64_t bar_low, bar_high;
    uint64_t bar_addr;

    bar_low = (uint64_t)pci_config_read(bus, slot, func, PCI_BAR0);
    
    if ((bar_low & 0x04) == 0) return 0;
    
    bar_high = (uint64_t)pci_config_read(bus, slot, func, PCI_BAR0 + 4);

    bar_addr = (bar_high << 32) | (bar_low & 0xFFFFFFF0);
    return bar_addr;
}

static void nvme_enable_device(uint8_t bus, uint8_t slot, uint8_t func) {
    uint32_t cmd;

    cmd = pci_config_read(bus, slot, func, PCI_COMMAND_REGISTER);
    cmd |= 0x0007;
    pci_config_write(bus, slot, func, PCI_COMMAND_REGISTER, cmd);
}



uint32_t os_pci_read_reg(uint64_t base, uint32_t offset) {
    return *(volatile uint32_t *)(base + offset);
}

void os_pci_write_reg(uint64_t base, uint32_t offset, uint32_t value) {
    *(volatile uint32_t *)(base + offset) = value;
}

void os_sleep_ms(uint32_t ms) {
    volatile uint64_t delay = ms * 100000;
    while (delay--);
}