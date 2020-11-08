; Defined in isr.c
[extern isrHandler]
[extern irqHandler]

isrCommonCode:
    pusha
    mov ax, ds
    push eax ; Push the current data segment
    mov ax, 0x10
    mov ds, ax ; Set segments to kernel data segment descriptor
    mov es, ax
    mov fs, ax
    mov gs, ax

	push esp ; Push a pointer to the interruptRegisters struct
    cld ; C code following the sysV ABI requires DF to be clear on function entry
    call isrHandler

    pop eax ; Remove the pointer
    pop eax ; Recover eax
    mov ds, ax ; Set segments to the original value
    mov es, ax
    mov fs, ax
    mov gs, ax
    popa
    add esp, 0x08 ; Cleans up the stuff pushed by the isrXX functions
    iret

irqCommonCode:
    pusha
    mov ax, ds
    push eax ; Push the current data segment
    mov ax, 0x10
    mov ds, ax ; Set segments to kernel data segment descriptor
    mov es, ax
    mov fs, ax
    mov gs, ax

	push esp ; Push a pointer to the interruptRegisters struct
    cld ; C code following the sysV ABI requires DF to be clear on function entry
    call irqHandler

    pop eax ; Remove the pointer
    pop eax
    mov ds, ax ; Set segments to the original value
    mov es, ax
    mov fs, ax
    mov gs, ax
    popa
    add esp, 0x08 ; Cleans up the stuff pushed by the isrXX functions
    iret

%macro ISR_NOERRCODE 1
global isr%1
isr%1:
    push byte 0
    push byte %1
    jmp isrCommonCode
%endmacro

%macro ISR_ERRCODE 1
global isr%1
isr%1:
    push byte %1
    jmp isrCommonCode
%endmacro

%macro IRQ 2
global irq%1
irq%1:
    push byte 0x00
    push byte %2
    jmp irqCommonCode
%endmacro

ISR_NOERRCODE 0
ISR_NOERRCODE 1
ISR_NOERRCODE 2
ISR_NOERRCODE 3
ISR_NOERRCODE 4
ISR_NOERRCODE 5
ISR_NOERRCODE 6
ISR_NOERRCODE 7
ISR_ERRCODE 8
ISR_NOERRCODE 9
ISR_ERRCODE 10
ISR_ERRCODE 11
ISR_ERRCODE 12
ISR_ERRCODE 13
ISR_ERRCODE 14
ISR_NOERRCODE 15
ISR_NOERRCODE 16
ISR_ERRCODE 17
ISR_NOERRCODE 18
ISR_NOERRCODE 19
ISR_NOERRCODE 20
ISR_ERRCODE 21
ISR_NOERRCODE 22
ISR_NOERRCODE 23
ISR_NOERRCODE 24
ISR_NOERRCODE 25
ISR_NOERRCODE 26
ISR_NOERRCODE 27
ISR_NOERRCODE 28
ISR_NOERRCODE 29
ISR_NOERRCODE 30
ISR_NOERRCODE 31

IRQ 00, 32
IRQ 01, 33
IRQ 02, 34
IRQ 03, 35
IRQ 04, 36
IRQ 05, 37
IRQ 06, 38
IRQ 07, 39
IRQ 08, 40
IRQ 09, 41
IRQ 0A, 42
IRQ 0B, 43
IRQ 0C, 44
IRQ 0D, 45
IRQ 0E, 46
IRQ 0F, 47