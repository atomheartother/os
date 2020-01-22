[org 0x7c00]
mov bp, 0x8000 ; Move the stack a bit away
mov sp, bp
mov bx, 0x9000 ; Load sectors at this address
mov dh, 2
call disk_load

mov dx, [0x9000]
call print_hex
call print_nl

mov dx, [0x9000 + 512]
call print_hex
call print_nl
jmp $ ; jump to current address = infinite loop

%include "bios_print.asm"
%include "bios_disk.asm"
; Fill with 510 zeros minus the size of the previous code
times 510-($-$$) db 0
dw 0xaa55

times 256 dw 0xdada ; sector 2 = 512 bytes
times 256 dw 0xface ; sector 3 = 512 bytes