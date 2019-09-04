	.file	"bubble.c"
	.text
	.globl	bubble_a
	.type	bubble_a, @function
bubble_a:
.LFB23:
	.cfi_startproc
	leaq	-1(%rsi), %r9
	jmp	.L2
.L4:
	leaq	8(%rdi,%rax,8), %rsi
	movq	(%rsi), %rcx
	leaq	(%rdi,%rax,8), %rdx
	movq	(%rdx), %r8
	cmpq	%r8, %rcx
	jge	.L3
	movq	%r8, (%rsi)
	movq	%rcx, (%rdx)
.L3:
	addq	$1, %rax
	jmp	.L5
.L6:
	movl	$0, %eax
.L5:
	cmpq	%r9, %rax
	jl	.L4
	subq	$1, %r9
.L2:
	testq	%r9, %r9
	jg	.L6
	rep ret
	.cfi_endproc
.LFE23:
	.size	bubble_a, .-bubble_a
	.globl	bubble_b
	.type	bubble_b, @function

bubble_b:
.LFB24:
	.cfi_startproc
	leaq	-8(%rdi,%rsi,8), %rsi
	jmp	.L8
.L10:
	movq	8(%rax), %rdx
	movq	(%rax), %rcx
	cmpq	%rcx, %rdx
	jge	.L9
	movq	%rcx, 8(%rax)
	movq	%rdx, (%rax)
.L9:
	addq	$8, %rax
	jmp	.L11
.L12:
	movq	%rdi, %rax
.L11:
	cmpq	%rsi, %rax
	jb	.L10
	subq	$8, %rsi
.L8:
	cmpq	%rdi, %rsi
	ja	.L12
	rep ret
	.cfi_endproc
.LFE24:
	.size	bubble_b, .-bubble_b
	.globl	bubble_c
	.type	bubble_c, @function

bubble_c:
.LFB25:
	.cfi_startproc
	jmp	.L14
.L16:
	movq	8(%rax), %rdx
	movq	(%rax), %rcx
	cmpq	%rcx, %rdx
	jge	.L15
	movq	%rcx, 8(%rax)
	movq	%rdx, (%rax)
.L15:
	addq	$8, %rax
	jmp	.L17
.L18:
	movq	%rdi, %rax
.L17:
	cmpq	%rax, %rsi
	ja	.L16
	subq	$8, %rsi
.L14:
	cmpq	%rdi, %rsi
	ja	.L18
	rep ret
	.cfi_endproc
.LFE25:
	.size	bubble_c, .-bubble_c
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"%ld "
	.text
	.globl	print_arr
	.type	print_arr, @function

print_arr:
.LFB26:
	.cfi_startproc
	pushq	%r12
	.cfi_def_cfa_offset 16
	.cfi_offset 12, -16
	pushq	%rbp
	.cfi_def_cfa_offset 24
	.cfi_offset 6, -24
	pushq	%rbx
	.cfi_def_cfa_offset 32
	.cfi_offset 3, -32
	movq	%rdi, %r12
	movq	%rsi, %rbp
	movl	$0, %ebx
	jmp	.L20
.L21:
	movslq	%ebx, %rax
	movq	(%r12,%rax,8), %rdx
	movl	$.LC0, %esi
	movl	$1, %edi
	movl	$0, %eax
	call	__printf_chk
	addl	$1, %ebx
.L20:
	movslq	%ebx, %rax
	cmpq	%rbp, %rax
	jl	.L21
	movl	$10, %edi
	call	putchar
	popq	%rbx
	.cfi_def_cfa_offset 24
	popq	%rbp
	.cfi_def_cfa_offset 16
	popq	%r12
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE26:
	.size	print_arr, .-print_arr
	.globl	main
	.type	main, @function
main:
.LFB27:
	.cfi_startproc
	subq	$104, %rsp
	.cfi_def_cfa_offset 112
	movq	%fs:40, %rax
	movq	%rax, 88(%rsp)
	xorl	%eax, %eax
	movq	$0, (%rsp)
	movq	$9, 8(%rsp)
	movq	$8, 16(%rsp)
	movq	$2, 24(%rsp)
	movq	$4, 32(%rsp)
	movq	$3, 40(%rsp)
	movq	$1, 48(%rsp)
	movq	$5, 56(%rsp)
	movq	$7, 64(%rsp)
	movq	$6, 72(%rsp)
	leaq	72(%rsp), %rsi
	movq	%rsp, %rdi
	call	bubble_c
	movl	$10, %esi
	movq	%rsp, %rdi
	call	print_arr
	movq	88(%rsp), %rdx
	xorq	%fs:40, %rdx
	je	.L24
	call	__stack_chk_fail
.L24:
	movl	$0, %eax
	addq	$104, %rsp
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE27:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 5.4.0-6ubuntu1~16.04.10) 5.4.0 20160609"
	.section	.note.GNU-stack,"",@progbits
