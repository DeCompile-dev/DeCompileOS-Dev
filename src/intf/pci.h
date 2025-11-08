#ifndef PCI_H
#define PCI_H

#include <stdint.h>
#include <stdbool.h>

uint32_t pci_config_read(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset);
void pci_config_write(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset, uint32_t value);

uint32_t os_pci_read_reg(uint64_t base, uint32_t offset);
void os_pci_write_reg(uint64_t base, uint32_t offset, uint32_t value);
void os_sleep_ms(uint32_t ms);

extern void map_page(uint64_t paddr, uint64_t vaddr, uint64_t flags);
#endif