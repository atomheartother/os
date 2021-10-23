; Magic number for multiboot
MAGIC equ 0x1BADB002
; Multiboot flags
MBALIGN equ 1 << 0 ; Align loaded modules on page boundaries
MEMINFO equ 1 << 1 ; Provide a memory map
FLAGS equ MBALIGN | MEMINFO
; Checksum so that CHECKSUM + MAGIC + FLAGS == 0 to prove we are multiboot
CHECKSUM equ -(MAGIC + FLAGS)

; Write our multiboot header
; Bootloader will search for this, aligned at a 32bit boundary
section .multiboot
align 4
  dd MAGIC
  dd FLAGS
  dd CHECKSUM

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
    jmp 0x08:.reloadSegments
.reloadSegments:
    mov ax, DATA_SEG ; Update segment registers to be in data segment
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    ; We should be good to go.
    ; Re-enable interrupts
    sti

    extern kernel_main
    call kernel_main ; Here goes nothing!
; Use this to infinite-loop without using up CPU
.hang: 
  cli ; Clear all interrupts
  hlt ; Wait for the next interrupt
  jmp .hang ; Jump back to .hang
.end:

%include "32b_gdt.asm"
