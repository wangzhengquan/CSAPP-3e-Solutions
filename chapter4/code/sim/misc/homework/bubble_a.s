	.file	"bubble_a.c"
	.text
	.globl	bubble_a
	.type	bubble_a, @function
bubble_a:
.LFB0:
	.cfi_startproc
	leaq	-1(%rsi), %r8       # count
	testq	%r8, %r8
	jle	.L1
	leaq	(%rdi,%rsi,8), %rsi # data_end
	jmp	.L3
.L5:
	movq	(%rax), %rdx
	movq	-8(%rax), %rcx
	cmpq	%rcx, %rdx          # cmp data[i+1] data[i]
	jge	.L4
	movq	%rcx, (%rax)         
	movq	%rdx, -8(%rax)      # swap
.L4:
	addq	$8, %rax            # i+1
	cmpq	%rsi, %rax         
	jne	.L5                   #&data[i] != &data[end]
.L6:
	subq	$8, %rsi            #data_end - 8
	subq	$1, %r8             #count - 1
	je	.L1                   #if count == 0
.L3:
#	testq	%r8, %r8
#	jle	.L6
	leaq	8(%rdi), %rax       # &data[1]
	jmp	.L5
.L1:
	rep ret
	.cfi_endproc
.LFE0:
	.size	bubble_a, .-bubble_a
	.ident	"GCC: (Ubuntu 5.4.0-6ubuntu1~16.04.10) 5.4.0 20160609"
	.section	.note.GNU-stack,"",@progbits
