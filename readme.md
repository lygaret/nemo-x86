# nemo-x86

a toy kernel, for funsies.

## to run

-   `git clone https://github.com/lygaret/nemo-x86.git`
-   `cd nemo-x86`
-   `make`
-   `bochs` (debugger already setup)

## dependencies

-   clang
-   make
-   bochs

## conceptual organization

* `ext/` 
    headers and implementation code for external formats, like
    multiboot and tar. liukely everything in here has it's own license.
* `kernel/`
    the kernel core klib functionality - task switching, vmm, message
    queues, locks/semaphores, etc.
* `service/`
    user space microkernel services provided by the system. disk access,
    file system, networking, etc. these all get plugged into the /svc
    namespace in the kernel's vfs
* `stdlib/`
    c stdlib, headers, etc. user space - uses kernel services
* `system/`
    hardware specific stuff, exposes a set of functionality the kernel can
    use to interface with the hardware - putc, context switching, page
    frame allocation, etc.

## licenses
-   Unless otherwise noted, code/assets/etc. are licensed under the [MIT License](./license.md).
-   `multiboot.h` is copied from grub 2, where it's MIT licensed - the licence is at the top of the file
