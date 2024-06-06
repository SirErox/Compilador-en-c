section .data
  a dd 1
  b dd 1
  msg db 'positivo',0
  len equ $ - msg

section .text
  global _start

_start:
  mov eax, [a];
  mov ebx, [b];

cmp eax,ebx;
je if1;
jne done;

if1:
  mov eax, 4;
  mov ebx, 1;
  mov ecx, msg;
  mov edx, len;
  int 80h;
  jmp done;
done:
    mov eax,1
    mov ebx,0
    int 80h
