#include <ext/multiboot.h>
#include <stdlib/stdlib.h>
#include <kernel/h/panic.h>
#include <kernel/h/pmm.h>
#include <kernel/h/screen.h>
#include <kernel/h/stdio.h>

uintptr_t * pagedir = (uintptr_t *) 0xFFFFF000;
uintptr_t pmm_free_stack = 0;
unsigned  pmm_free_total = 0;

extern uintptr_t * _kernend;

static inline void flush_tlb(uintptr_t addr) {
   asm volatile("invlpg (%0)" ::"r" (addr) : "memory");
}

// allocate a frame
// todo - wrap with a mutex or something
void pmm_alloc_frame(page_t *page, bool kernel, bool writeable) {
  uintptr_t frameaddr = pmm_free_stack;

  if (frameaddr == 0) {
    kpanic("out of free memory!");
  }

  page->present = 1;
  page->rw      = writeable ? 1 : 0;
  page->user    = kernel ? 0 : 1;
  page->frame   = frameaddr;


}

// Function to deallocate a frame.
void free_frame(page_t *page)
{
   u32int frame;
   if (!(frame=page->frame))
   {
       return; // The given page didn't actually have an allocated frame!
   }
   else
   {
       clear_frame(frame); // Frame is now free again.
       page->frame = 0x0; // Page now doesn't have a frame.
   }
}

void pmm_initialize(uintptr_t mmap_addr, size_t mmap_length) {
  kprintf("pmm_init ... ");

  // walk the free areas of the memory map, and manage these pages on the stack
  // space below 16mb is kernel reserved
  // space above is first come first served
  // kernel can steal space from shared heap if needed 

  uintptr_t virtaddr  = 0x60000000;
  unsigned  pdindex   = virtaddr >> 22;

  uintptr_t entry_addr = mmap_addr;
  while (entry_addr < (mmap_addr + mmap_length)) {
    multiboot_memory_map_t * entry = (multiboot_memory_map_t *)entry_addr;

    // skip processing if it's not marked available
    if (entry->type != MULTIBOOT_MEMORY_AVAILABLE)
      goto next;

    uintptr_t addr = (uintptr_t)entry->addr;
    uintptr_t end  = addr + entry->len;

    // skip to 4Mb aligned start
    if ((addr & ~0x3FFFFF) != addr) {
      addr = (addr + 1) & ~0x3FFFFF;
    }

    // loop over every 4k page, and mark it
    for (; addr < end; addr += 0x4000) {
      unsigned offset = addr % 0x400000;

      // if the current address is a 4Mb page boundary, temporarily map the page
      if (offset == 0) {
        pagedir[pdindex] = addr | 0b10000011;
        flush_tlb(virtaddr);
      }

      // skip over the bios, kernel
      if (addr < (uintptr_t)&_kernend) {
        continue;
      }

      // write the current stack header to the page
      if (pmm_free_stack != 0) {
        *(uint32_t *)(virtaddr + offset) = pmm_free_stack;
      }

      // update the stack pointer
      pmm_free_stack  = addr;
      pmm_free_total += 1;
    }

  next:
    entry_addr = entry_addr + entry->size + sizeof(uintptr_t);
  }

  // done! clear out the temp pagedir
  pagedir[pdindex] = 0;
  flush_tlb(virtaddr);

  kprintf(" ok! (%d total pages, %d total Mb)\n", pmm_free_total, (pmm_free_total * 0x4000) / 0x100000);
}
