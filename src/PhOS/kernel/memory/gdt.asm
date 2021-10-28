[bits 32]

gdtr dw 0
     dd 0

[extern loadGDT]

loadGDT:
    mov eax, [esp + 4]
    mov [gdtr + 2], eax
    ;mov ax, [esp + 8]
    ;dec ax
    ;mov [gdtr], ax
    ;lgdt [gdtr]
    ;ret
