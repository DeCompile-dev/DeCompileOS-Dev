; CODE.ASM - Public LIBRAY

global code_fuction
global code_variable

section .data
code_variable:
    dd 0xDEADBEEF

section .text
code_fuction:
    mov ebx, [code_variable]
    mov eax, 45
    add eax, ebx
    cmp eax, 0
    jmp end

end:
    ret

; CODE.ASM - Public LIBRAY