#define MEM_IMPL // we define extern structs here

#include <ext/multiboot.h>
#include <stdlib/stdlib.h>
#include <kernel/h/stdio.h>
#include <kernel/h/mem.h>
#include <kernel/h/panic.h>

#define mmap_entry_t multiboot_memory_map_t

// todo: does the multiboot mmap handle mmio areas? if so, we can harvest this space
extern char kern_start;
extern char kern_end;

// the memory map address from the multiboot header
uintptr_t mmap_addr;
size_t    mmap_length;

// the current entry within the multiboot header
// and the offset of the next free page
uintptr_t curr_entry_addr;
size_t    curr_offset;

// the interface
// forward declarations to keep this at the top

mem_frame_t frame_allocate();
void        frame_deallocate(mem_frame_t);

const mem_frame_allocator_t mem_frame_allocator = {
  .allocate   = frame_allocate,
  .deallocate = frame_deallocate
};

/* initialize the memory system
   @param mb_mmap_addr: the address of the multiboot memory map
   @param mb_mmap_length: the length of the multiboot memory map
 */
void mem_initialize(uintptr_t mb_mmap_addr, size_t mb_mmap_length) {
  mmap_addr       = mb_mmap_addr;
  mmap_length     = mb_mmap_length;
  curr_entry_addr = mmap_addr;
  curr_offset     = 0;

  kprintf("mem_init ... %h, %d\n", mmap_addr, mmap_length);
}

// private implementation stuff

mem_frame_t frame_allocate() {

  // walk the memory map, looking for free space (from the current entry+offset)

  while (curr_entry_addr < (mmap_addr + mmap_length)) {
    mmap_entry_t *curr_entry = (mmap_entry_t *)curr_entry_addr;

    // is this mmap entry unreserved and big enough?

    bool reserved    = curr_entry->type != MULTIBOOT_MEMORY_AVAILABLE;
    bool unavailable = (curr_offset + PAGE_SIZE) > curr_entry->len;

    if (reserved || unavailable) {
      curr_entry_addr += curr_entry->size + sizeof(uintptr_t);
      curr_offset      = 0;
      continue;
    }

    // black list the kernel

    bool in_kern_mem = (curr_entry->addr + curr_offset + PAGE_SIZE) <= (uintptr_t)&kern_end;
    if (in_kern_mem) {
      curr_offset += PAGE_SIZE;
      continue;
    }

    // otherwise, return a frame!

    mem_frame_t frame = { .addr = curr_entry->addr + curr_offset };
    curr_offset += PAGE_SIZE;
    return frame;
  }

  // we've walked past the end of the memory map, so there's nothing we can return :(

  kpanic("out of memory!");
}

void frame_deallocate(mem_frame_t frame) {
}
