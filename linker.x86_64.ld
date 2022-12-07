OUTPUT_FORMAT(elf64-x86-64)
OUTPUT_ARCH(i386:x86-64)
 
PHDRS
{
    text    PT_LOAD    FLAGS((1 << 0) | (1 << 2)); /* Execute + Read */
    rodata  PT_LOAD    FLAGS((1 << 2));            /* Read only */
    data    PT_LOAD    FLAGS((1 << 1) | (1 << 2)); /* Write + Read */
}
 
SECTIONS
{
    . = 0xffffffff80000000;
    
    text_start_addr = .;
    .text : { *(.text .text.*) } : text
    text_end_addr = .;
    . += CONSTANT(MAXPAGESIZE);
    
    rodata_start_addr = .;
    .rodata : { *(.rodata .rodata.*) } : rodata
    rodata_end_addr = .;
    . += CONSTANT(MAXPAGESIZE);

    data_start_addr = .;
    .data : { *(.data .data.*) } : data
    .bss : { *(COMMON) *(.bss .bss.*) } : data
    data_end_addr = .;

    /DISCARD/ : { *(.eh_frame) *(.note .note.*) }

    .init_array : 
    {
        PROVIDE_HIDDEN (__init_array_start = .);
        KEEP (*(SORT_BY_INIT_PRIORITY(.init_array.*) SORT_BY_INIT_PRIORITY(.ctors.*)))
        KEEP (*(.init_array .ctors))
        PROVIDE_HIDDEN (__init_array_end = .);
    }
}