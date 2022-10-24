OUTPUT_FORMAT(elf32-i386)

ENTRY(kernel_entry)

SECTIONS
{
    . = 0x100000;

    .entry               : { __entry_start = .;       *(.entry)    }
    .text               : { __text_start = .;       *(.text)    }
    .data               : { __data_start = .;       *(.data)    }    
    .rodata             : { __rodata_start = .;     *(.rodata)  }    
    .bss                : { __bss_start = .;        *(.bss);    }    __bss_end = .;
}