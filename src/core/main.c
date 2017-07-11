#include <stdlib/stdlib.h>
#include <ext/multiboot.h>

extern void _panic();
void kpanic() {
  return _panic();
}

void kernel_main(uint32_t bootmagic, multiboot_info_t *bootinfo) {
  if (bootmagic != MULTIBOOT_BOOTLOADER_MAGIC)
    _panic();

  // todo:
  // setup gdt
  // setup paging
  // setup isrs/irq mapping/pic remap
}
