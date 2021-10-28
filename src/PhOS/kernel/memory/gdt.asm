bits 32
default rel

[extern load_gdt]
load_gdt:
    cli
    lgdt [gdt_descriptor]
    call reload_segments
    ret

reload_segments:
    jmp 0x08:.reload_CS
    .reload_CS:
    mov   ax, 0x10
    mov   ds, ax
    mov   es, ax
    mov   fs, ax
    mov   gs, ax
    mov   ss, ax
    ret

gdt_descriptor:
dw gdt_end - gdt_start - 1
gdt_start:
gdt_null_entry:
    dq 0x0000000000000000
gdt_kernel_code_entry:
    dw 0xFFFF ; limit_low 	
    dw 0x0000 ; base_low
    db 0x00   ; base_middle
    db 0x9A   ; access
    db 0xCF   ; limit_high & flags
    db 0x00   ; base_high
gdt_kernel_data_entry: 	
    dw 0xFFFF ; limit_low
    dw 0x0000 ; base_low
    db 0x00	  ; base_middle	
    db 0x92   ; access
    db 0xCF   ; limit_high & flags
    db 0x00   ; base_high
gdt_user_code_entry: 	
    dw 0xffff ; limit_low
    dw 0x0000 ; base_low
    db 0x00	  ; base_middle	
    db 0xFA   ; access
    db 0xCF   ; limit_high & flags
    db 0x00   ; base_high
gdt_user_data_entry: 	
    dw 0xFFFF ; limit_low
    dw 0x0000 ; base_low
    db 0x00	  ; base_middle	
    db 0xF2   ; access
    db 0xCF   ; limit_high & flags
    db 0x00   ; base_high
gdt_end:
