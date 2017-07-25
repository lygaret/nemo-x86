#ifndef __kernel_pmm_h
#define __kernel_pmm_h

#include <stdlib/stdlib.h>
#include <ext/multiboot.h>

typedef struct {
   unsigned  present    : 1;   // Page present in memory
   unsigned  rw         : 1;   // Read-only if clear, readwrite if set
   unsigned  user       : 1;   // Supervisor level only if clear
   unsigned  accessed   : 1;   // Has the page been accessed since last refresh?
   unsigned  dirty      : 1;   // Has the page been written to since last refresh?
   unsigned  unused     : 7;   // Amalgamation of unused and reserved bits
   uintptr_t frame      : 20;  // Frame address (shifted right 12 bits)
} page_t;

typedef struct {
   page_t pages[1024];
} page_table_t;

typedef struct {
  page_table_t *pagetables[1024];
} page_directory_t;

void pmm_initialize(uintptr_t mmap, size_t length);
uintptr_t pmm_alloc(size_t count);
void pmm_free(uintptr_t addr);

#endif
