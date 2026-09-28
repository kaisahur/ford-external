.CODE

PUBLIC nt_read_virtual_mem
nt_read_virtual_mem PROC
	mov r10, rcx
	mov eax, 63
	syscall
	ret
nt_read_virtual_mem ENDP

PUBLIC nt_write_virtual_mem
nt_write_virtual_mem PROC
	mov r10, rcx
	mov eax, 58
	syscall
	ret
nt_write_virtual_mem ENDP

PUBLIC nt_allocate_virtual_mem
nt_allocate_virtual_mem PROC
    mov r10, rcx
    mov eax, 18h    
    syscall
    ret
nt_allocate_virtual_mem ENDP

END