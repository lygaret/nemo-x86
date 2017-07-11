#define ASM_FILE 1
#include <core/boot.h>

        // reserved data space 
        .section .bss
        .align   0x10 
stack_top:
        .space STACK_SIZE, 0x00
heap_top:
        .space HEAP_SIZE, 0x00

        .section .data
        .align 0x1000
pagedir:
        .space 1024 * 4, 0x00000000

        // code
        .section .text
        .global _start
        .global _panic

        // macro for translating virtual addresses (pre-paging)
        .macro physaddr virtaddr, dest
        mov \virtaddr, \dest
        subl $KERNEL_VIRT_BASE, \dest
        .endm

        // entry point
        // eax - multiboot info
        // ebx - multiboot magic
_start: 
        physaddr $stack_top, %esp
        pusha

        // enable paging
        // along with PSE (so we can use 4Mb pages)

        physaddr $pagedir, %eax

        // install identity map
        movl $0x00000083,  (%eax)
        movl $0x00000083,  (KERNEL_PAGE_INDEX * 4)(%eax)

        // install recursive mapping
        movl %eax,  (1023 * 4)(%eax)
        orl  $0x01, (1023 * 4)(%eax)

        // drop the pagedir into cr3
        mov %eax, %cr3       

        // enable PSE (for 4Mb pages)
        mov %cr4, %ebx
        or  $0x00000010, %ebx
        mov %ebx, %cr4

        // enable paging flag in cr0
        // PAGING ENABLED!
        mov %cr0, %ebx
        or  $0x80000000, %ebx
        mov %ebx, %cr0

        // move the stack to the new place
        add $KERNEL_VIRT_BASE, %esp
        popa

        // far jump to virtual address space
        lea _start_virtual, %ecx
        jmp *%ecx

_start_virtual:
        // get to C (addrs are virtual now)
        push %ebx // multiboot info
        push %eax // multiboot magic
        call kernel_main

        // if `kernel_main` returns, panic!
        // and name it so others can use it too
_panic:
        cli
        hlt
        jmp _panic
