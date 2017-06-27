# ---------------------- Beginning of Generated Code --------------------
        .file   "foo"
        .text
.globl test
        .type   test, @function
test:
.LFB0:
	.cfi_startproc
	pushq	%rbp              # save base pointer on stack
	.cfi_def_cfa_offset 16
	movq	%rsp, %rbp        # move stack pointer to base pointer
	.cfi_offset 6, -16
	.cfi_def_cfa_register 6
        subq	$32, %rsp 	  # make space for this stack frame
	movq	%rbx, %r9        # save %rbx (callee-saved) in %r9
# ------------------------- begin Your code -----------------------------
	jmp	.L0 			#  jump 
go:
	pushq	%rbp			# save base pointer on stack
	movq	%rsp, %rbp		# move stack pointer to base pointer
	subq	$32, %rsp		# make space for this stack frame
	movl	%edi,-32(%rbp)     	#  %edi -> a
	movl	$1,%eax         	#  1 -> %eax
	movl	-32(%rbp),%ebx     	#  a -> %ebx
	subl	%eax,%ebx         	#  %ebx - %eax -> %ebx
	movl	%ebx,-24(%rbp)     	#  %ebx -> b
	movl	$2,%eax         	#  2 -> %eax
	movl	-32(%rbp),%ebx     	#  a -> %ebx
	subl	%eax,%ebx         	#  %ebx - %eax -> %ebx
	movl	%ebx,-20(%rbp)     	#  %ebx -> c
	movl	-32(%rbp),%eax     	#  a -> %eax
	movl	$1,%ebx         	#  1 -> %ebx
	cmpl	%ebx,%eax           	#  compare %eax - %ebx
	je	.L1 			#  jump if     ==
	jmp	.L2 			#  jump 
.L1:
	movl	$1,%eax         	#  1 -> %eax
	movl	%eax,-28(%rbp)     	#  %eax -> _go
	jmp	.L3 			#  jump 
.L2:
	movl	-32(%rbp),%eax     	#  a -> %eax
	movl	$2,%ebx         	#  2 -> %ebx
	cmpl	%ebx,%eax           	#  compare %eax - %ebx
	je	.L4 			#  jump if     ==
	jmp	.L5 			#  jump 
.L4:
	movl	$1,%eax         	#  1 -> %eax
	movl	%eax,-28(%rbp)     	#  %eax -> _go
	jmp	.L6 			#  jump 
.L5:
	movl	-24(%rbp),%ebx     	#  b -> %ebx
	movl	%ebx,%edi         	#  %ebx -> %edi
	call	go              	#  go()
	movl	%eax,%ebx         	#  %eax -> %ebx
	movl	%ebx,-28(%rbp)     	#  %ebx -> _go
	movl	-20(%rbp),%ebx     	#  c -> %ebx
	movl	%ebx,%edi         	#  %ebx -> %edi
	call	go              	#  go()
	movl	%eax,%ebx         	#  %eax -> %ebx
	movl	-28(%rbp),%esp     	#  _go -> %esp
	addl	%esp,%ebx         	#  %ebx + %esp -> %ebx
	movl	%ebx,-28(%rbp)     	#  %ebx -> _go
.L6:
.L3:
	movl	-28(%rbp),%eax     	#  _go -> %eax
	movq    %rbp, %rsp
	popq    %rbp
	ret
.L0:
	movl	$9,%ebx         	#  9 -> %ebx
	movl	%ebx,%edi         	#  %ebx -> %edi
	call	go              	#  go()
	movl	%eax,%ebx         	#  %eax -> %ebx
	movl	%ebx,-32(%rbp)     	#  %ebx -> x
	movl	-32(%rbp),%eax     	#  x -> %eax
	movl	%eax,%edi         	#  %eax -> %edi
	call	writeln              	#  writeln()
# ----------------------- begin Epilogue code ---------------------------
	movq	%r9, %rbx        # restore %rbx (callee-saved) from %r9
        leave
        ret
        .cfi_endproc
.LFE0:
        .size   test, .-test
# ----------------- end Epilogue; Literal data follows ------------------
        .section        .rodata

        .ident  "Compiler Principle - Spring 2017"
