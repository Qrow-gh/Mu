ENTRY(KrnlEntry)

SECTIONS {
    KRNL_VMA = .;

    __KRNL_START__ = .;

    .text : AT(ADDR(.text) - KRNL_VMA) {
        __KRNL_CODE__ = .;
        *(.text)
        . = ALIGN(4096);
    }

    .data : AT(ADDR(.data) - KRNL_VMA) {
        __KRNL_DATA__ = .;
        *(.data)
        . = ALIGN(4096);
    }

    .rodata : AT(ADDR(.rodata) - KRNL_VMA) {
        __KRNL_RODATA__ = .;
        *(.data)
        .ALIGN(4096);
    }

    .eh_frame : AT(ADDR(.eh_frame) - KRNL_VMA) {
        __KRNL_EH_FRAME__ = .;
        *(.eh_frame)
        . = ALIGN(4096);
    }

    .bss : AT(ADDR(.bss) - KRNL_VMA) {
        __KRNL_BSS__ = .;
        *(.bss)
        *(.stck)
        *(.heap)
        *(COMMON)
        . = ALIGN(4096);
    }

    __KRNL_END__ = .;

    /DISCARD/ : {
        *(.common)
    }
}