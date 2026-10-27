ENTRY(_start)

PHDRS
{
    text PT_LOAD FLAGS(5); /* Read + Execute */
    data PT_LOAD FLAGS(6); /* Read + Write */
}

SECTIONS {
    . = 1M;

    .text : {
        *(.multiboot)
        *(.text)
        *(.rodata*)
    } :text

	. = ALIGN(4096);

    .data : {
        *(.data)
		*(.gdt)
    } :data

    .bss  : {
        *(.bss)
		*(COMMON)
    } :data
}
