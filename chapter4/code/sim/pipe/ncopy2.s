	.file	"ncopy2.c"
	.text
	.globl	ncopy
	.type	ncopy, @function
ncopy:
.LFB23:
	.cfi_startproc
	leaq	-3(%rdx), %r9
	testq	%r9, %r9
	jle	.L10
	movl	$0, %eax
	movl	$0, %ecx
.L7:
	movq	(%rdi), %r8
	movq	%r8, (%rsi)
	testq	%r8, %r8
	setg	%r8b
	movzbl	%r8b, %r8d
	addq	%r8, %rax
	movq	8(%rdi), %r8
	movq	%r8, 8(%rsi)
	testq	%r8, %r8
	setg	%r8b
	movzbl	%r8b, %r8d
	addq	%r8, %rax
	movq	16(%rdi), %r8
	movq	%r8, 16(%rsi)
	testq	%r8, %r8
	setg	%r8b
	movzbl	%r8b, %r8d
	addq	%r8, %rax
	movq	24(%rdi), %r8
	movq	%r8, 24(%rsi)
	testq	%r8, %r8
	setg	%r8b
	movzbl	%r8b, %r8d
	addq	%r8, %rax
	addq	$32, %rdi
	addq	$32, %rsi
	addq	$4, %rcx
	cmpq	%rcx, %r9
	jg	.L7
	jmp	.L2
.L10:
	movl	$0, %eax
	movl	$0, %ecx
.L2:
	cmpq	%rcx, %rdx
	jle	.L8
.L11:
	movq	(%rdi), %r8
	movq	%r8, (%rsi)
	addq	$1, %rcx
	cmpq	%rcx, %rdx
	jne	.L11
.L8:
	rep ret
	.cfi_endproc
.LFE23:
	.size	ncopy, .-ncopy
	.ident	"GCC: (Ubuntu 5.4.0-6ubuntu1~16.04.11) 5.4.0 20160609"
	.section	.note.GNU-stack,"",@progbits
