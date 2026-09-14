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

    .data : { 
        *(.data) 
        *(.gdt)
    } :data
    
    .bss  : { 
        *(.bss) 
    } :data
}
