section .data
  a dd 1
  b dd 1
  c dd 1

section .text
  global _start

_start:
  mov eax, [a];
  mov ebx, [b];
  mov ecx, [c];

