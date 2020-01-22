; Switch to 32bit protected mode
switch_to_pm:
    cli ; clear interrupts
    lgdt [gdt_descriptor] ; Load gdt
    mov eax, cr0 ; Load cr0
    or eax, 0x1 ; enable bit 0x1 (Protected mode)
    mov cr0, eax ; Write to cr0
    jmp CODE_SEG:init_pm

[bits 32]
; We are now using 32bit instructions
init_pm:
    mov ax, DATA_SEG ; Update segment registers to be in data segment
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    ; Move the stack at the top of the free space
    mov ebp, 0x90000
    mov esp, ebp

    call BEGIN_PM