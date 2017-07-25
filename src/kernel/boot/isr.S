        .section .text

        .global idt_flush
idt_flush:
        lidt idt_ptr
        ret

        // when an exceptions provide an errcode, it's already on the stack
        .macro isr_with_errcode num
        .global isr_\num
isr_\num\():
        cli
        push $\num
        jmp isr_common_stub
        .endm

        // provide a dummy error code to maintain calling convention
        .macro isr_no_errcode num
        .global isr_\num
isr_\num\():
        cli
        push $0    // dummy error code
        push $\num // isr number
        jmp isr_common_stub
        .endm

        // isr_common_stub(num, errcode) 
isr_common_stub:
        pusha

        // pass stack pointer, (we have struct of regs)
        push %esp
        call isr_dispatch
        pop %esp

        popa         
        add $8, %esp // clean up pushed error code and isr number
        sti          // re-enable interrupts (disabled in unique wrappers) 
        iret

        // exceptions
        isr_no_errcode   0  // divide by zero
        isr_no_errcode   1  // debug
        isr_no_errcode   2  // non maskable interrupt
        isr_no_errcode   3  // breakpoint
        isr_no_errcode   4  // overflow
        isr_no_errcode   5  // bound range exceeded
        isr_no_errcode   6  // invalid opcode
        isr_no_errcode   7  // device not available
        isr_with_errcode 8  // double fault
        isr_no_errcode   9  // unused / coprocessor overrun
        isr_with_errcode 10 // invalid tss
        isr_with_errcode 11 // segment not present
        isr_with_errcode 12 // stack segment fault
        isr_with_errcode 13 // general protection fault
        isr_with_errcode 14 // page fault
        isr_no_errcode   15 // unused / reserved
        isr_no_errcode   16 // x87 floating point exception
        isr_with_errcode 17 // alignment check
        isr_no_errcode   18 // machine check
        isr_no_errcode   19 // simd floating point exception
        isr_no_errcode   20 // virtualization exception
        
        // 21-31 unused / reserved
        isr_no_errcode   21
        isr_no_errcode   22
        isr_no_errcode   23
        isr_no_errcode   24
        isr_no_errcode   25
        isr_no_errcode   26
        isr_no_errcode   27
        isr_no_errcode   28
        isr_no_errcode   29
        isr_no_errcode   30
        isr_no_errcode   31

        // 32-47 are remapped IRQs
        isr_no_errcode   32
        isr_no_errcode   33
        isr_no_errcode   34
        isr_no_errcode   35
        isr_no_errcode   36
        isr_no_errcode   37
        isr_no_errcode   38
        isr_no_errcode   39
        isr_no_errcode   40
        isr_no_errcode   41
        isr_no_errcode   42
        isr_no_errcode   43
        isr_no_errcode   44
        isr_no_errcode   45
        isr_no_errcode   46
        isr_no_errcode   47
