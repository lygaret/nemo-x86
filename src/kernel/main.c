#include <ext/multiboot.h>
#include <stdlib/stdlib.h>
#include <kernel/kernel.h>

void kernel_main(uint32_t bootmagic, multiboot_info_t *bootinfo) {
  if (bootmagic != MULTIBOOT_BOOTLOADER_MAGIC) {
    kpanic("bootmagic error! %h", bootmagic);
  }

  // base initialization
  isr_initialize();
  mem_initialize(bootinfo->mmap_addr, bootinfo->mmap_length);

  // device installation
  timer_install(1000/*hz*/);
  keyboard_install();

  enable_interrupts();

  kprintf("let's get some pages!\n");
  for (int i = 0; i < 5; i++) {
    mem_frame_t frame = mem_frame_allocator.allocate();
    kprintf("page number: %h\n", frame.addr);
  }

  kprintf("didn't explode!\n");
  
  while(1) {
    asm volatile ("hlt");
  }
}
