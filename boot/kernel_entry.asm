[bits 32]
START:
[extern _start]
    call _start
    jmp $

extern _idt
extern _HandleISR1
extern _HandleISR12

global _isr1, _isr12
global _LoadIDT

IDTDesc:
    dw 2048
    dd _idt

_isr1:
    pusha
    call _HandleISR1
    popa
    iret

_isr12:
    pusha
    call _HandleISR12
    popa
    iret

_LoadIDT:
    lidt[IDTDesc]
    sti
    ret