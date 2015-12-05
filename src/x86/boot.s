[BITS 32]

MBALIGN     equ  1 << 0                 ; align loaded modules on page boundaries
MEMINFO     equ  1 << 1                 ; provide memory map
FLAGS       equ  MBALIGN | MEMINFO      ; this is the Multiboot 'flag' field
MAGIC       equ  0x1BADB002             ; 'magic number' lets bootloader find the header
CHECKSUM    equ -(MAGIC + FLAGS)        ; checksum of above, to prove we are multiboot

section .multiboot
jmp _start  ; If we didn't boot from multiboot, just start
align 4
dd MAGIC
dd FLAGS
dd CHECKSUM

section .bootstrap_stack, nobits
align 4
stack_bottom:
resb 16384
stack_top:

section .text
global _start
global halt
extern __init
extern __fini
extern main
_start:
    ; Set up the stack
    mov esp, stack_top
    ; Push the pointer to the Multiboot information structure.
    push   ebx
    ; Push the magic value. 
    push   eax
    
    call __init
    call main
halt:
    call __fini
    cli
    hlt
hang:
    jmp hang

