#include <system/defines.h>

        .section .bss
        .global bootstack
        .align 0x10
bootstack:
        .space STACK_SIZE

        .section .data

        .global bootpagedir
        .global bootpagetable
        .global gdt_ptr

        .align 0x10
gdt:
        .quad 0x0000000000000000        // null descriptor
        .quad 0x00CF9A000000FFFF        // kernel code (4Gb, ring 0, exec)
        .quad 0x00CF92000000FFFF        // kernel data (4Gb, ring 0, no exec)
        .quad 0x00CFFA000000FFFF        // user   code (4Gb, ring 3, exec)
        .quad 0x00CFF2000000FFFF        // user   data (4Gb, ring 3, no exec)
gdt_ptr:
        .word gdt_ptr - gdt - 1
        .long gdt

        .align 0x1000                   // 4Mb pages,
bootpagedir:
        .long 0x00000083                // 0x00000000 -> 0x00000000 - 0x0003FFFF
        .fill (PAGE_INDEX - 1), 4
        .long 0x00000083                // 0xE0000000 -> 0x00000000 - 0x0003FFFF
        .fill (1023 - PAGE_INDEX), 4
        .long bootpagedir + 1           // recursive pagedir
