.intel_syntax noprefix
.text
    .globl _start, syscall
    _start:
        xor rbp,rbp
        pop rdi       // set argc
        mov rsi,rsp   // set argv
        mov rdx,rdi   // set envp
        add rdx,1
        shl rdx,3
        add rdx,rsp
        add rsp,-8    // prepare stack for main
        and spl,0xf8
        call main@PLT
	mov rdi,rax // syscall
        mov rax,60
        syscall
    syscall:
        mov rax,rdi
        mov rdi,rsi
        mov rsi,rdx
        mov rdx,rcx
        mov r10,r8
        mov r8,r9
        syscall
        ret
