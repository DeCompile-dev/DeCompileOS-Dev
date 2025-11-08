#include "memory.h"
#include "register.h"
#include <stddef.h> 

__attribute__((aligned(PAGE_SIZE))) page_entry_t pml4_table_data[ENTRIES_PER_TABLE];
page_entry_t *pml4_table = pml4_table_data; 

#define MAX_FRAMES 32768
uint8_t frame_bitmap[MAX_FRAMES / 8]; 

void init_page_frame_allocator() {
    for (int i = 0; i < MAX_FRAMES / 8; i++) {
        frame_bitmap[i] = 0;
    }

    for (int i = 0; i < (8 * 1024 * 1024 / PAGE_SIZE); i++) {
        frame_bitmap[i / 8] |= (1 << (i % 8)); 
    }
}

void *pfa_allocate_frame() {
    for (int i = 0; i < MAX_FRAMES; i++) {
        if (!(frame_bitmap[i / 8] & (1 << (i % 8)))) {
            frame_bitmap[i / 8] |= (1 << (i % 8));
            return (void *)((uint64_t)i * PAGE_SIZE);
        }
    }
    return NULL;
}

static page_entry_t *get_next_table(page_entry_t *entry) {
    if (!(*entry & PTE_PRESENT)) {
        void *new_table_phys = pfa_allocate_frame();

        if (new_table_phys == NULL) {
             return NULL;
        }

        *entry = ((uint64_t)new_table_phys & ~0xFFF) | PTE_PRESENT | PTE_WRITEABLE;
    }

    uint64_t next_table_phys_addr = *entry & ~0xFFF;
    return (page_entry_t *)next_table_phys_addr;
}

void map_page(uint64_t vaddr, uint64_t paddr, uint64_t flags) {
    uint64_t pml4_index = (vaddr >> 39) & 0x1FF;
    uint64_t pdpt_index = (vaddr >> 30) & 0x1FF;
    uint64_t pd_index   = (vaddr >> 21) & 0x1FF;
    uint64_t pt_index   = (vaddr >> 12) & 0x1FF;

    page_entry_t *pdpt, *pd, *pt;
    
    page_entry_t *pml4_entry = &pml4_table[pml4_index]; 

    pdpt = get_next_table(pml4_entry); 
    if (pdpt == NULL) return; 

    pd = get_next_table(&pdpt[pdpt_index]);
    if (pd == NULL) return;

    pt = get_next_table(&pd[pd_index]);
    if (pt == NULL) return;

    pt[pt_index] = (paddr & ~0xFFF) | flags;
}

void enable_paging() {
    write_cr3((uint64_t)pml4_table); 

    uint64_t cr0 = read_cr0();
    cr0 |= (1 << 31);
    write_cr0(cr0);
    
    uint64_t cr4 = read_cr4();
    cr4 |= (1 << 5);
    write_cr4(cr4);
}