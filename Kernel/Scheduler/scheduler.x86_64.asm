;
;  Created by vitriol1744 on 07.12.2022.
;  Copyright (c) 2022-2022, Szymon Zemke <Vitriol1744@gmail.com>
;
;  SPDX-License-Identifier: GPL-3
;
use64
section .text

%macro popaq 0
    pop r15
    pop r14
    pop r13
    pop r12
    pop r11
    pop r10
    pop r9
    pop r8
    pop rsi
    pop rdi
    pop rbp
    pop rdx
    pop rcx
    pop rbx
    pop rax
%endmacro

global switch_process
switch_process:
    mov rsp, rdi
    popaq
    add rsp, 0x10
    swapgs
    sti
    iretq
