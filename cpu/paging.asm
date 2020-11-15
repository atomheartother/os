global initPaging
initPaging:
    ; Put pageDirectory in eax
    push ebp
    mov ebp, esp
    mov DWORD eax, [esp+8]
    mov cr3, eax
    ; Enable paging
    mov eax, cr0
    or eax, 0x80000001
    mov cr0, eax
    pop ebp
    ret