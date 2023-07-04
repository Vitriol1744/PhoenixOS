OUTPUT_FORMAT(elf64-aarch64)
OUTPUT_ARCH(elf64-littleaarch64:aarch64)

ENTRY(kernelStart)

PHDRS
{
    text    PT_LOAD    FLAGS((1 << 0) | (1 << 2));
    rodata  PT_LOAD    FLAGS((1 << 2));
    data    PT_LOAD    FLAGS((1 << 1) | (1 << 2));
}

SECTIONS
{
    . = 0xffffffff80000000;

    text_start_addr = .;
    .text : { *(.text .text.*) } :text
    text_end_addr = .;
    . += CONSTANT(MAXPAGESIZE);

    rodata_start_addr = .;
    .rodata : { *(.rodata .rodata.*) } :rodata
    rodata_end_addr = .;
    . += CONSTANT(MAXPAGESIZE);

    .init_array :
    {
        PROVIDE_HIDDEN(__init_array_start = .);
        KEEP(*(SORT_BY_INIT_PRIORITY(.init_array.*) SORT_BY_INIT_PRIORITY(.ctors.*)))
        KEEP(*(.init_array .ctors))
        PROVIDE_HIDDEN(__init_array_end = .);
    }
    . += CONSTANT(MAXPAGESIZE);

    data_start_addr = .;
    .data : { *(.data .data.*) } :data

    /* NOTE: .bss needs to be the last thing mapped to :data, otherwise lots of */
    /* unnecessary zeros will be written to the binary. */
    /* If you need, for example, .init_array and .fini_array, those should be placed */
    /* above this. */
    .bss : { *(COMMON) *(.bss .bss.*) } :data
    data_end_addr = .;

    /* Discard .note.* and .eh_frame since they may cause issues on some hosts. */
    /DISCARD/ : { *(.eh_frame) *(.note .note.*) }
}