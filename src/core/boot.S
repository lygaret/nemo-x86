#define ASM_FILE 1
#include <core/boot.h>

        // untangle prepaging virtual addresses
        .macro paddr vaddr, dest
        mov \vaddr, \dest
        subl $VIRT_BASE, \dest
        .endm

        // retangle virtual addresses
        .macro vaddr dest
        add $VIRT_BASE, \dest
        .endm

        // OR a mask into a control register
        .macro orcr mask, dest, tmp
        mov \dest, \tmp
        or  \mask, \tmp
        mov \tmp, \dest
        .endm

        // code

        .section .text
        .global _start
        .global _panic
_start:
        paddr $bootstack, %esp

        // kernel args on the stack
        push %ebx
        push %eax

        // paging
        paddr $bootpagedir, %eax
        mov   %eax, %cr3
        orcr  $0x00000010, %cr4, %eax   // enable 4Mb pages (PSE bit in CR4)
        orcr  $0x80000000, %cr0, %eax   // enable paging (PG bit in CR0)

        // jump to kernel address space
        lea 1f, %eax
        jmp *%eax
1:      vaddr %esp

        // gdt
        lgdt gdt_ptr
        jmp $0x08, $1f
1:      call _flush_segments

        // idt

        // start working in c
        call kernel_main

_panic:
        cli
        hlt
        jmp _panic

_flush_segments:
        push %eax
        mov $0x10, %ax
        mov %ax, %ds
        mov %ax, %es
        mov %ax, %fs
        mov %ax, %gs
        mov %ax, %ss
        pop %eax
        ret

        .section .bss
        .global bootstack
        .align 0x10
bootstack:
        .space STACK_SIZE

        .section .data
        .global bootpagedir
        .global bootpagetable
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
