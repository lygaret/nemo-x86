#include <ext/multiboot.h>
#include <stdlib/stdlib.h>
#include <kernel/kernel.h>

void kernel_main(uint32_t bootmagic, multiboot_info_t *bootinfo) {
  if (bootmagic != MULTIBOOT_BOOTLOADER_MAGIC)
    kpanic("bootmagic error! %h", bootmagic);

  kprintf("Hello?\n");

  // todo:
  // setup gdt
  // setup paging
  // setup isrs/irq mapping/pic remap
}
