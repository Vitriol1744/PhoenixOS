bits 16
org 0x7c00

jmp bpb
nop

times 32-($$-$) db 0
skip_bpb:
    cli

times 510-($$-$) db 0
dw 0xaa55