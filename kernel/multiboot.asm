; Magic number for multiboot
MAGIC equ 0x1BADB002
; Multiboot flags
MBALIGN equ 1 << 0 ; Align loaded modules on page boundaries
MEMINFO equ 1 << 1 ; Provide a memory map
AOUTKLUDGE equ 1 << 16 ; We will provide address information

FLAGS equ MBALIGN | MEMINFO ; | AOUTKLUDGE 
; Checksum so that CHECKSUM + MAGIC + FLAGS == 0 to prove we are multiboot
CHECKSUM equ -(MAGIC + FLAGS)

extern _textSectionStart
extern _dataSectionEnd
extern _bssSectionEnd

; Write our multiboot header
; Bootloader will search for this, aligned at a 32bit boundary
section .multiboot
mbootHeader:
align 4
; Magic values, required
  dd MAGIC
  dd FLAGS
  dd CHECKSUM
; Addresses, if we set flags bit 16
;  dd mbootHeader
;  dd _textSectionStart
;  dd _dataSectionEnd
;  dd _bssSectionEnd
;  dd _start

; Make a 16KiB stack, 16-bit aligned
section .bss
align 16
stack_top:
resb 16384
stack_bottom:

section .text
global _start:function (_start.end - _start)
_start:
	; The bootloader has loaded us into 32-bit protected mode on a x86
	; machine. Interrupts are disabled. Paging is disabled.
    ; Set up the stack so C can function
    mov esp, stack_bottom

    cli ; Disable interrupts
    lgdt [gdt_descriptor] ; Load gdt
    jmp CODE_SEG:.reloadSegments
.reloadSegments:
    mov ax, DATA_SEG ; Update segment registers to be in data segment
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    extern kernel_main
    call kernel_main ; Here goes nothing!
; Use this to infinite-loop without using up CPU
.hang: 
  hlt ; Wait for the next interrupt
  jmp .hang ; Jump back to .hang
.end:

%include "32b_gdt.asm"
