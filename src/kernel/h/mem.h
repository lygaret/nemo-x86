#ifndef __kernel_mem_h
#define __kernel_mem_h

#include <stdlib/stdlib.h>

#define PAGE_SIZE 0x4000

typedef struct {
  uintptr_t addr;
} mem_frame_t;

typedef struct {
  mem_frame_t (*allocate)();
  void        (*deallocate)(mem_frame_t frame);
} mem_frame_allocator_t;

// initialize the memory manager with the mmap from multiboot
void mem_initialize(uintptr_t mmap_addr, size_t mmap_length);

// unless we're defining the allocator, it's an extern symbol
#ifndef MEM_IMPL
extern const mem_frame_allocator_t mem_frame_allocator;
#endif

#endif
