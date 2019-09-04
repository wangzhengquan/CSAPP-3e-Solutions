	.file	"switchv.c"
	.text
	.globl	switchv
	.type	switchv, @function
switchv:
.LFB23:
	.cfi_startproc
	cmpq	$2, %rdi
	je	.L3
	cmpq	$2, %rdi
	jg	.L4
	movl	$2730, %eax
	testq	%rdi, %rdi
	je	.L5
	jmp	.L2
.L4:
	movl	$3276, %eax
	cmpq	$3, %rdi
	je	.L5
	cmpq	$5, %rdi
	jne	.L2
.L3:
	movl	$3003, %eax
	ret
.L2:
	movl	$3549, %eax
.L5:
	rep ret
	.cfi_endproc
.LFE23:
	.size	switchv, .-switchv
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"idx = %ld, val = 0x%lx\n"
	.text
	.globl	main
	.type	main, @function

main:
.LFB24:
	.cfi_startproc
	pushq	%rbx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	movq	$-1, %rbx
.L9:
	movq	%rbx, %rdi
	call	switchv
	movq	%rax, %rcx
	movq	%rbx, %rdx
	movl	$.LC0, %esi
	movl	$1, %edi
	movl	$0, %eax
	call	__printf_chk
	addq	$1, %rbx
	cmpq	$7, %rbx
	jne	.L9
	movl	$0, %eax
	popq	%rbx
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE24:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 5.4.0-6ubuntu1~16.04.10) 5.4.0 20160609"
	.section	.note.GNU-stack,"",@progbits
