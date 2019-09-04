	.file	"struct.c"
	.text
	.globl	rsum_list
	.type	rsum_list, @function
rsum_list:
.LFB23:
	.cfi_startproc
	movl	$0, %eax
	testq	%rdi, %rdi
	je	.L6
	pushq	%rbx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	movq	%rdi, %rbx
	movq	8(%rdi), %rdi
	call	rsum_list
	addq	(%rbx), %rax
	popq	%rbx
	.cfi_restore 3
	.cfi_def_cfa_offset 8
.L6:
	rep ret
	.cfi_endproc
.LFE23:
	.size	rsum_list, .-rsum_list
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"%ld\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB24:
	.cfi_startproc
	subq	$72, %rsp
	.cfi_def_cfa_offset 80
	movq	%fs:40, %rax
	movq	%rax, 56(%rsp)
	xorl	%eax, %eax
	movq	$30, (%rsp)
	movq	$0, 8(%rsp)
	movq	$20, 16(%rsp)
	movq	%rsp, 24(%rsp)
	movq	$10, 32(%rsp)
	leaq	16(%rsp), %rax
	movq	%rax, 40(%rsp)
	leaq	32(%rsp), %rdi
	call	rsum_list
	movq	%rax, %rdx
	movl	$.LC0, %esi
	movl	$1, %edi
	movl	$0, %eax
	call	__printf_chk
	movq	56(%rsp), %rcx
	xorq	%fs:40, %rcx
	je	.L8
	call	__stack_chk_fail
.L8:
	movl	$0, %eax
	addq	$72, %rsp
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE24:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 5.4.0-6ubuntu1~16.04.10) 5.4.0 20160609"
	.section	.note.GNU-stack,"",@progbits
