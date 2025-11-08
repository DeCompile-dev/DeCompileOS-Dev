#include "register.h"

// --- CR3 ---

void write_cr3(uint64_t cr3) {
    __asm__ volatile (
        "mov %0, %%cr3"
        :
        : "r" (cr3)
        : "memory"
    );
}

uint64_t read_cr3() {
    uint64_t cr3_val;
    __asm__ volatile (
        "mov %%cr3, %0"
        : "=r" (cr3_val)
        :
        : "memory"
    );
    return cr3_val;
}

// --- CR0 ---

void write_cr0(uint64_t cr0) {
    __asm__ volatile (
        "mov %0, %%cr0"
        :
        : "r" (cr0)
        : "memory"
    );
}

uint64_t read_cr0() {
    uint64_t cr0_val;
    __asm__ volatile (
        "mov %%cr0, %0"
        : "=r" (cr0_val)
        :
        : "memory"
    );
    return cr0_val;
}

// --- CR4 ---

void write_cr4(uint64_t cr4) {
    __asm__ volatile (
        "mov %0, %%cr4"
        :
        : "r" (cr4)
        : "memory"
    );
}

uint64_t read_cr4() {
    uint64_t cr4_val;
    __asm__ volatile (
        "mov %%cr4, %0"
        : "=r" (cr4_val)
        :
        : "memory"
    );
    return cr4_val;
}