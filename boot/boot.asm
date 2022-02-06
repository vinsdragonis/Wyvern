[org 0x7c00]
[bits 16]

section code

; .init:
;     mov eax, 0xb800
;     mov es, eax
;     mov eax, 0 ; counter variable set to 0
;     mov ebx, 0 ; index of character in the string being printed
;     mov ecx, 0 ; actual address of character on the screen
;     mov dl, 0 ; store actual value being printed to the screen

; .clear:
;     mov byte[es:eax], 0 ; move blank character to current text address
;     inc eax
;     mov byte[es:eax], 0x0A ; move bg color and character color to the next address
;     inc eax

;     cmp eax, 2 * 25 * 80

;     jl .clear

; mov eax, startupmsg
; mov ecx, 0 * 2 * 80
; call .print

; jmp .switch

; .print:
;     mov dl, byte [eax + ebx]

;     cmp dl, 0
;     je .print_end

;     mov byte [es:ecx], dl
    
;     inc ebx
;     inc ecx
;     inc ecx

;     jmp .print

; .print_end:
;     ret

.switch:
    mov ax, 0x4f01 ; querying the VBE
    mov cx, 0x117 ; required mode
    mov bx, 0x0800 ; offset for the VBE infrastructure
    mov es, bx
    mov di, 0x00
    int 0x10

    ; make switch to graphics mode

    mov ax, 0x4f02
    mov bx, 0x117
    int 0x10

    xor ax, ax
    mov ds, ax
    mov es, ax

    mov bx, 0x1000 ; location of the code being loaded from the hard disk
    mov ah, 0x02
    mov al, 21 ; number of sectors to read from the hard disk
    mov ch, 0x00
    mov dh, 0x00
    mov cl, 0x02
    int 0x13

    cli ; turn off interrupts
    lgdt [gdt_descriptor] ; load GDT table

    mov eax, cr0
    or eax, 0x1
    mov cr0, eax ; make the switch

    jmp code_seg:protected_start

startupmsg: db 'Welcome to Wyvern!', 0

[bits 32]
protected_start:
    mov ax, data_seg
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    ; update stack pointer

    mov ebp, 0x90000
    mov esp, ebp

    call 0x1000
    jmp $

gdt_begin:
gdt_null_descriptor:
    dd 0x00
    dd 0x00
gdt_code_seg:
    dw 0xffff
    dw 0x00
    db 0x00
    db 10011010b
    db 11001111b
    db 0x00
gdt_data_seg:
    dw 0xffff
    dw 0x00
    db 0x00
    db 10010010b
    db 11001111b
    db 0x00
gdt_end:
gdt_descriptor:
    dw gdt_end - gdt_begin - 1
    dd gdt_begin
code_seg equ gdt_code_seg - gdt_begin
data_seg equ gdt_data_seg - gdt_begin

times 510 - ($ - $$) db 0x00 ; pads the file with 0s, making it the right size

db 0x55
db 0xaa