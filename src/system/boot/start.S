#define ASM_FILE 1
#include <system/defines.h>
#include <ext/multiboot.h>
#include "./macros.inc"

        .global _start
        .global _panic

_start:
        paddr $bootstack, %esp

        // kernel args onto the stack (multiboot)
        push %ebx
        push %eax

        // paging
        paddr $bootpagedir, %eax
        mov   %eax, %cr3
        orcr  $0x00000010, %cr4, %eax
        orcr  $0x80000000, %cr0, %eax

        // jump into the new address space
        addl $VIRT_BASE, %esp
        lea 1f, %eax
        jmp *%eax

        // gdt
1:      lgdt gdt_ptr
        jmp $0x08, $1f          // reset the %cs register
1:      mov $0x10, %ax          // reset the segment registers
        mov %ax, %ds
        mov %ax, %es
        mov %ax, %fs
        mov %ax, %gs
        mov %ax, %ss

        // idt

        // jump to c
        call kernel_main

_panic: 
        cli
        hlt
        jmp _panic
