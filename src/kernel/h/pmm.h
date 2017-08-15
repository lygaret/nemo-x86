#ifndef __kernel_pmm_h
#define __kernel_pmm_h

#include <stdlib/stdlib.h>
#include <ext/multiboot.h>

typedef struct {
  unsigned present  : 1;   // Page present in memory
  unsigned rw       : 1;   // Read-only if clear, readwrite if set
  unsigned user     : 1;   // Supervisor level only if clear
  unsigned accessed : 1;   // Has the page been accessed since last refresh?
  unsigned dirty    : 1;   // Has the page been written to since last refresh?
  unsigned unused   : 7;   // Amalgamation of unused and reserved bits
  uintptr_t frame   : 20;  // Frame address (shifted right 12 bits)
} page_table_entry_t;

typedef struct {
  bool present  : 1; // Page present in memory
  bool rw       : 1; // Read-only if clear, readwrite if set
  bool user     : 1;   // Supervisor level only if clear
  bool writethu : 1;   // Has the page been accessed since last refresh?
  bool uncached : 1;   // Has the page been accessed since last refresh?
  bool accessed : 1;   // Has the page been accessed since last refresh?
  bool reserved : 1; // Has the page been written to since last refresh?
  bool hugepage : 1; // 1 = 4Mb, 0 = 4Kb (1 = direct, 0 = table reference)
  unsigned unused   : 4;   // Amalgamation of unused and reserved bits
  uintptr_t frame   : 20; 
} page_directory_entry_t;

typedef struct {
  page_table_entry_t pages[1024];
} page_table_t;

typedef struct {
  page_table_t * tables[1024];
} page_directory_t;

extern page_directory_entry_t * bootpagedir;

void pmm_initialize(uintptr_t mmap, size_t length);
void pmm_map_page(uintptr_t paddr, uintptr_t vaddr, unsigned flags);

void * kmalloc(uint32_t size, bool align, uintptr_t *phys);


#endif
