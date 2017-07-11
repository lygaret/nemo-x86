#include <stdlib/stdlib.h>
#include <ext/multiboot.h>

// this occurs in physical memory, pre paging
void kernel_boot(uint32_t bootmagic, multiboot_info_t *bootinfo) {
  char c = bootinfo->boot_loader_name;
  bootinfo->boot_loader_name = c + 3;
}
