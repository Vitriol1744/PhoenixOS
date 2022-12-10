/*
 * Created by vitriol1744 on 25.11.2022.
 * Copyright (c) 2022-2022, Szymon Zemke <Vitriol1744@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3
 */
use64

section .text
extern raiseInterrupt

%macro pushaq 0
    push rax
    push rbx
    push rcx
    push rdx
    push rbp
    push rdi
    push rsi
    push r8
    push r9
    push r10
    push r11
    push r12
    push r13
    push r14
    push r15
%endmacro

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

%macro interrupt_handler 1
align 16
interrupt_handler_%1:
    cli
    swapgs
    cld
%if %1 != 8 && %1 != 10 && %1 != 11 && %1 != 12 && %1 != 13 && %1 != 14 && %1 != 17 && %1 != 30
    push 0
%endif
    push %1
    pushaq
    mov rdi, rsp
    call raiseInterrupt
    popaq
    add rsp, 0x10
    swapgs
    sti
    iretq
%endmacro

%assign i 0
%rep 256
interrupt_handler i
%assign i i+1
%endrep

section .data

%macro get_exception_addr 1
dq interrupt_handler_%1
%endmacro

global interrupt_handlers
interrupt_handlers:
%assign i 0
%rep 256
	get_exception_addr i
%assign i i+1
%endrep
