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
