#ifndef MEMORY_H
#define MEMORY_H

#include "stdint.h"

#define PAGE_SIZE       0x1000
#define PTE_PRESENT     0x001
#define PTE_WRITEABLE   0x002
#define PTE_USER        0x004

#define ENTRIES_PER_TABLE 512

typedef uint64_t page_entry_t;
typedef page_entry_t page_table_t[ENTRIES_PER_TABLE];

extern page_entry_t *pml4_table; 

void init_memory_map();
void init_page_frame_allocator();
void *pfa_allocate_frame();
void map_page(uint64_t vaddr, uint64_t paddr, uint64_t flags);
void enable_paging();

#endif