[org 0x7c00]
    mov bp, 0x9000 ; set the stack
    mov sp, bp
    
    mov bx, MSG_REAL_MODE
    call real_print

    call switch_to_pm
    jmp $

%include "bios_print.asm"
%include "32b_gdt.asm"
%include "32b_print.asm"
%include "32b_switch.asm"

[bits 32]
BEGIN_PM:
    ; After the 32bit switch, this is executed
    mov ebx, MSG_PROT_MODE
    call print_vga
    jmp $

MSG_REAL_MODE db "Started in 16-bit real mode", 0
MSG_PROT_MODE db "Loaded 32-bit protected mode", 0

; Padding: fill with 510 zeros minus the size of the previous code
times 510-($-$$) db 0
dw 0xaa55