[bits 32]

VIDEO_MEMORY equ 0xb8000
WHITE_ON_BLACK equ 0x0f ; the color byte for each character

; Print string in ebx to VGA memory
print_vga:
    pusha
    mov edx, VIDEO_MEMORY
    mov ah, WHITE_ON_BLACK
.loop:
    mov al, [ebx]
    cmp al, 0
    je .done
    mov [edx], ax
    inc ebx
    add edx, 2
    jmp .loop
.done:
    popa
    ret