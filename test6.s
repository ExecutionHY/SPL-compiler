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
gcd:
	pushq	%rbp			# save base pointer on stack
	movq	%rsp, %rbp		# move stack pointer to base pointer
	subq	$32, %rsp		# make space for this stack frame
	movl	%edi,-32(%rbp)     	#  %edi -> a
	movl	%esi,-28(%rbp)     	#  %esi -> b
	movl	-28(%rbp),%eax     	#  b -> %eax
	movl	$0,%ebx         	#  0 -> %ebx
	cmpl	%ebx,%eax           	#  compare %eax - %ebx
	je	.L1 			#  jump if     ==
	jmp	.L2 			#  jump 
.L1:
	movl	-32(%rbp),%eax     	#  a -> %eax
	movl	%eax,-24(%rbp)     	#  %eax -> _gcd
	jmp	.L3 			#  jump 
.L2:
	movl	-32(%rbp),%eax     	#  a -> %eax
	movl	-28(%rbp),%ebx     	#  b -> %ebx
	movl	%eax,%eax         	#  %eax -> %eax
	movl	%eax,%edx         	#  %eax -> %edx
	call	mod              	#  mod()
	movl	%eax,-20(%rbp)     	#  %eax -> c
	movl	-28(%rbp),%ebx     	#  b -> %ebx
	movl	%ebx,%edi         	#  %ebx -> %edi
	movl	-20(%rbp),%ebx     	#  c -> %ebx
	movl	%ebx,%esi         	#  %ebx -> %esi
	call	gcd              	#  gcd()
	movl	%eax,%ebx         	#  %eax -> %ebx
	movl	%ebx,-24(%rbp)     	#  %ebx -> _gcd
.L3:
	movl	-24(%rbp),%eax     	#  _gcd -> %eax
	movq    %rbp, %rsp
	popq    %rbp
	ret
.L0:
	movl	$36,%ebx         	#  36 -> %ebx
	movl	%ebx,%edi         	#  %ebx -> %edi
	movl	$9,%ebx         	#  9 -> %ebx
	movl	%ebx,%esi         	#  %ebx -> %esi
	call	gcd              	#  gcd()
	movl	%eax,%ebx         	#  %eax -> %ebx
	movl	%ebx,-32(%rbp)     	#  %ebx -> ans
	movl	$6,%ebx         	#  6 -> %ebx
	movl	%ebx,%edi         	#  %ebx -> %edi
	movl	$3,%ebx         	#  3 -> %ebx
	movl	%ebx,%esi         	#  %ebx -> %esi
	call	gcd              	#  gcd()
	movl	%eax,%ebx         	#  %eax -> %ebx
	movl	%ebx,-28(%rbp)     	#  %ebx -> tmp
	movl	-28(%rbp),%eax     	#  tmp -> %eax
	movl	-32(%rbp),%ebx     	#  ans -> %ebx
	imull	%ebx		#  %eax * %ebx -> %eax
	movl	%eax,-32(%rbp)     	#  %eax -> ans
	movl	-32(%rbp),%eax     	#  ans -> %eax
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
