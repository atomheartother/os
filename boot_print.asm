; Very simple printing subroutines

;; Print a null-terminated string
;; bx: Address of string
print:
    pusha
    mov ah, 0x0e ; TTY mode
.loop:
    mov al, [bx] ; Load byte    
    cmp al, 0
    je .done
    int 0x10 ; Print
    inc ebx
    jmp .loop
.done:
    popa
    ret

;; Print a newline
print_nl:
    pusha
    mov ah, 0x0e
    mov al, 0xa
    int 0x10
    mov al, 0x0d
    int 0x10
    popa
    ret

; Print a hex number (2 bytes)
; dx contains the value
print_hex:
    pusha
    mov cx, 0
.loop:
    mov ax, dx
    ; Get the right character for this digit
    and ax, 0x000f ; 0x1234 -> 0x0004
    add al, 0x30
    cmp al, 0x3A
    jl .load_str
    add al, 0x07
.load_str:
    ; Put the character in the str
    mov bx, HEX_OUT + 5
    sub bx, cx
    mov [bx], al

    ror dx, 4 ; Rotate by one hex digit
    inc cx
    cmp cx, 4 ; Loop once per digit
    jne .loop
    mov bx, HEX_OUT
    call print
    popa
    ret

HEX_OUT:
    db '0x0000', 0