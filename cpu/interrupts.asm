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

    call isrHandler

    pop eax
    mov ds, ax ; Set segments to the original value
    mov es, ax
    mov fs, ax
    mov gs, ax
    popa
    add esp, 0x08 ; Cleans up the stuff pushed by the isrXX functions
    sti ; Re-enable interrupts
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

    call irqHandler
    pop ebx

    pop eax
    mov ds, ax ; Set segments to the original value
    mov es, ax
    mov fs, ax
    mov gs, ax
    popa
    add esp, 0x08 ; Cleans up the stuff pushed by the isrXX functions
    sti ; Re-enable interrupts
    iret

global isr00
global isr01
global isr02
global isr03
global isr04
global isr05
global isr06
global isr07
global isr08
global isr09
global isr0A
global isr0B
global isr0C
global isr0D
global isr0E
global isr0F
global isr10
global isr11
global isr12
global isr13
global isr14
global isr15
global isr16
global isr17
global isr18
global isr19
global isr1A
global isr1B
global isr1C
global isr1D
global isr1E
global isr1F

global irq00
global irq01
global irq02
global irq03
global irq04
global irq05
global irq06
global irq07
global irq08
global irq09
global irq0A
global irq0B
global irq0C
global irq0D
global irq0E
global irq0F

isr00:
    cli
    push byte 0
    push byte 0x0
    jmp isrCommonCode

isr01:
    cli
    push byte 0
    push byte 0x1
    jmp isrCommonCode

isr02:
    cli
    push byte 0
    push byte 0x2
    jmp isrCommonCode

isr03:
    cli
    push byte 0
    push byte 0x3
    jmp isrCommonCode

isr04:
    cli
    push byte 0
    push byte 0x4
    jmp isrCommonCode

isr05:
    cli
    push byte 0
    push byte 0x5
    jmp isrCommonCode

isr06:
    cli
    push byte 0
    push byte 0x6
    jmp isrCommonCode

isr07:
    cli
    push byte 0
    push byte 0x7
    jmp isrCommonCode

isr08:
    cli
    push byte 0
    push byte 0x8
    jmp isrCommonCode

isr09:
    cli
    push byte 0
    push byte 0x9
    jmp isrCommonCode

isr0A:
    cli
    push byte 0xA
    jmp isrCommonCode

isr0B:
    cli
    push byte 0xB
    jmp isrCommonCode

isr0C:
    cli
    push byte 0xC
    jmp isrCommonCode

isr0D:
    cli
    push byte 0xD
    jmp isrCommonCode

isr0E:
    cli
    push byte 0xE
    jmp isrCommonCode

isr0F:
    cli
    push byte 0
    push byte 0xF
    jmp isrCommonCode

isr10:
    cli
    push byte 0
    push byte 0x10
    jmp isrCommonCode

isr11:
    cli
    push byte 0
    push byte 0x11
    jmp isrCommonCode

isr12:
    cli
    push byte 0
    push byte 0x12
    jmp isrCommonCode

isr13:
    cli
    push byte 0
    push byte 0x13
    jmp isrCommonCode

isr14:
    cli
    push byte 0
    push byte 0x14
    jmp isrCommonCode

isr15:
    cli
    push byte 0
    push byte 0x15
    jmp isrCommonCode

isr16:
    cli
    push byte 0
    push byte 0x16
    jmp isrCommonCode

isr17:
    cli
    push byte 0
    push byte 0x17
    jmp isrCommonCode

isr18:
    cli
    push byte 0
    push byte 0x18
    jmp isrCommonCode

isr19:
    cli
    push byte 0
    push byte 0x19
    jmp isrCommonCode

isr1A:
    cli
    push byte 0
    push byte 0x1A
    jmp isrCommonCode

isr1B:
    cli
    push byte 0
    push byte 0x1B
    jmp isrCommonCode

isr1C:
    cli
    push byte 0
    push byte 0x1C
    jmp isrCommonCode

isr1D:
    cli
    push byte 0
    push byte 0x1D
    jmp isrCommonCode

isr1E:
    cli
    push byte 0
    push byte 0x1E
    jmp isrCommonCode

isr1F:
    cli
    push byte 0
    push byte 0x1F
    jmp isrCommonCode

irq00:
    cli
    push byte 0x00
    push byte 0x00
    jmp irqCommonCode

irq01:
    cli
    push byte 0x00
    push byte 0x01
    jmp irqCommonCode

irq02:
    cli
    push byte 0x00
    push byte 0x02
    jmp irqCommonCode

irq03:
    cli
    push byte 0x00
    push byte 0x03
    jmp irqCommonCode

irq04:
    cli
    push byte 0x00
    push byte 0x04
    jmp irqCommonCode

irq05:
    cli
    push byte 0x00
    push byte 0x05
    jmp irqCommonCode

irq06:
    cli
    push byte 0x00
    push byte 0x06
    jmp irqCommonCode

irq07:
    cli
    push byte 0x00
    push byte 0x07
    jmp irqCommonCode

irq08:
    cli
    push byte 0x00
    push byte 0x08
    jmp irqCommonCode

irq09:
    cli
    push byte 0x00
    push byte 0x09
    jmp irqCommonCode

irq0A:
    cli
    push byte 0x00
    push byte 0x0A
    jmp irqCommonCode

irq0B:
    cli
    push byte 0x00
    push byte 0x0B
    jmp irqCommonCode

irq0C:
    cli
    push byte 0x00
    push byte 0x0C
    jmp irqCommonCode

irq0D:
    cli
    push byte 0x00
    push byte 0x0D
    jmp irqCommonCode

irq0E:
    cli
    push byte 0x00
    push byte 0x0E
    jmp irqCommonCode

irq0F:
    cli
    push byte 0x00
    push byte 0x0F
    jmp irqCommonCode