ENTRY(kernelMain)

SECTIONS
{
    kernel_physical_offset = 0xFFFFFFFF80000000;
    . = kernel_physical_offset + 0x200000;

    .stivale2hdr ALIGN(4K) : AT (ADDR (.stivale2hdr) - kernel_physical_offset)
    {
        *(.stivale2hdr)
    }

    .text ALIGN(4K) : AT (ADDR (.text) - kernel_physical_offset)
    {
        *(.entry)
        *(.text .text.*)
    }

    .rodata ALIGN(4K) : AT (ADDR (.rodata) - kernel_physical_offset)
    {
        constructors_start = .;
        *(.ctors)
        constructors_end = .;

        destructors_start = .;
        *(.dtors)
        destructors_end = .;

        *(.rodata*)
    }

    .data ALIGN(4K) : AT (ADDR (.data) - kernel_physical_offset)
    {
        *(.data .data.*)
    }

    .bss ALIGN(4K) : AT (ADDR (.bss) - kernel_physical_offset)
    {
        *(COMMON)
        *(.bss .bss.*)
    }
}