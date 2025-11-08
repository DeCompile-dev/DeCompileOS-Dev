#ifndef REGISTER_H
#define REGISTER_H

#include "stdint.h"

// CR3 (Adres Paging Structure - PML4)
void write_cr3(uint64_t cr3);
uint64_t read_cr3();

// CR0 (Control Register 0)
void write_cr0(uint64_t cr0);
uint64_t read_cr0();

// CR4 (Control Register 4)
void write_cr4(uint64_t cr4);
uint64_t read_cr4();

#endif // REGISTER_H