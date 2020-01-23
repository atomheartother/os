; Subroutines related to reading from disk

; Load dh sectors from drive dl into es:bx
disk_load:
    pusha
    push dx
    mov ah, 0x02
    mov al, dh
    mov ch, 0x00 ; Cylinder number
    mov cl, 0x02 ; Read from first sector after boot
    mov dh, 0x00 ; Head number
    int 0x13
    jc disk_error
    pop dx ; Retrieve number of sectors we intended to read
    cmp al, dh
    jne sectors_err
    popa
    ret

disk_error:
    ; Error in disk read
    mov bx, DISK_ERR
    call real_print
    mov dh, ah ; dh: Error code. dl: Disk drive that caused it
    call real_print_hex
    call real_print_nl
    jmp disk_err_loop

sectors_err:
    ; Wrong number of sectors read
    mov bx, SECTORS_ERR
    call real_print
    call real_print_nl

disk_err_loop:
    ; Loop infinitely, this is a fatal error
    jmp $

DISK_ERR:
    db 'Disk read err: ', 0
SECTORS_ERR:
    db 'Incorrect number of sectors read', 0