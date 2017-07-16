#ifndef __core_boot_h
#define __core_boot_h

#define VIRT_BASE   0xE0000000
#define PHYS_ADDR   0x00100000
#define PAGE_INDEX  0x380 // (VIRT_BASE + PHYS_ADDR) >> 22

#define STACK_SIZE         0x4000 // 16k
#define HEAP_SIZE          0x4000 // 16k

#ifndef ASM_FILE

#endif  // ASM_FILE
#endif  // __core_boot_h
