[org 0x7c00]
; Where we'll load the kernel. Very important!
; Make sure to tell the kernel about this.
KERNEL_OFFSET equ 0x1000

    ; The BIOS gives us the boot drive in dl. Save it
    mov [BOOT_DRIVE], dl
    mov bp, 0x9000
    mov sp, bp

    mov bx, MSG_REAL_MODE 
    call real_print
    call real_print_nl

    call load_kernel ; read the kernel from disk
    call switch_to_pm ; switch to protected mode
    jmp $

%include "bios_print.asm"
%include "bios_disk.asm"
%include "32b_gdt.asm"
%include "32b_print.asm"
%include "32b_switch.asm"

[bits 16]
load_kernel:
    mov bx, MSG_LOAD_KERNEL
    call real_print
    call real_print_nl    

    mov bx, KERNEL_OFFSET
    mov dh, 16 ; Read 16 sectors to cover large future kernel
    mov dl, [BOOT_DRIVE] ; On drive given by BIOS
    call disk_load
    ret

[bits 32]
BEGIN_PM:
    ; After the 32bit switch, this is executed
    mov ebx, MSG_PROT_MODE
    call print_vga
    call KERNEL_OFFSET ; Here goes nothing
    jmp $

MSG_REAL_MODE db "Started in 16-bit real mode", 0
MSG_PROT_MODE db "Switched to 32-bit protected mode", 0
MSG_LOAD_KERNEL db "Loading kernel into memory", 0
BOOT_DRIVE db 0 ; Boot drive identifier, saved at startup

; Padding: fill with 510 zeros minus the size of the previous code
times 510-($-$$) db 0
dw 0xaa55