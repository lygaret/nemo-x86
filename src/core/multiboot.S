#define ASM_FILE 1
#include <ext/multiboot.h>

        .set MULTIBOOT_FLAGS,    MULTIBOOT_PAGE_ALIGN | MULTIBOOT_MEMORY_INFO
        .set MULTIBOOT_CHECKSUM, -(MULTIBOOT_HEADER_MAGIC + MULTIBOOT_FLAGS)

        // multiboot header
        .section .multiboot
        .align 4
        .long MULTIBOOT_HEADER_MAGIC
        .long MULTIBOOT_FLAGS
        .long MULTIBOOT_CHECKSUM
