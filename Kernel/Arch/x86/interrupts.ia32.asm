use32
section .text
extern raiseException

%macro exception 1
align 16
exception%1:
    cld
%if %1 == 8 || %1 == 10 || %1 == 11 || %1 == 12 || %1 == 13 || %1 == 14 || %1 == 17 || %1 == 30
    ; for these exceptions we can pop error code from the stack
    pop eax
%else
    xor eax, eax
%endif
    ; error code
    push eax
    ; exception vector
    push %1
    call raiseException
    pop eax
    pop eax
    pop ebp
    iret
%endmacro

%assign i 0
%rep 32
exception i
%assign i i+1
%endrep

section .rodata
%macro get_exception_addr 1
dd exception%1
%endmacro

global exception_handlers
exception_handlers:
%assign i 0
%rep 32
get_exception_addr i
%assign i i+1
%endrep
