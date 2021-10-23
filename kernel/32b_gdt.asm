; Define the GDTs for 32b mode addressing
gdt_start:
    dd 0 ; Start with 4 null bytes
    dd 0
gdt_code:
    dw 0xffff ; Size of segment bits 0-15
    dw 0 ; Address bits 0-15
    db 0 ; Address bits 16-23
    db 10011010b ; Flags (1001), then Type (1010)
    db 11001111b ; Flags (1100), then Size bits 16-19
    db 0 ; Address bits 24-31

gdt_data:
    dw 0xffff ; Size of segment bits 0-15
    dw 0 ; Address bits 0-15
    db 0 ; Address bits 16-23
    db 10010010b ; Flags (1001), then Type (0010) <- Different, this is a DATA segment!
    db 11001111b ; Flags (1100), then Size bits 16-19
    db 0 ; Address bits 24-31

gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1 ; 16b ize of GDT (-1 because GDT size start at 1, not 0)
    dd gdt_start ; 32b address

; Constants for using the actual GDT segments
CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start