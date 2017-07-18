#include <ext/multiboot.h>
#include <kernel/kernel.h>
#include <stdlib/stdlib.h>
#include <system/system.h>

void kernel_main(uint32_t bootmagic, multiboot_info_t *bootinfo) {
  if (bootmagic != MULTIBOOT_BOOTLOADER_MAGIC)
    kpanic("bootmagic error! %h", bootmagic);

  pic_remap(32, 38);
  isr_install();

  enable_interrupts();
  while(1) {
    asm volatile ("hlt");
  }
}
