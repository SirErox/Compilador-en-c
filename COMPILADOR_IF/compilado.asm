section .data
    a dd 1
    b dd 1

section .text
    global _start

_start:

done:
    mov eax,1
    mov ebx,0
    int 80h
