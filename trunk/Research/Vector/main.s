	.file	"main.cpp"
	.text
	.p2align 4,,15
.globl _Z9__rdtsc__v
	.type	_Z9__rdtsc__v, @function
_Z9__rdtsc__v:
.LFB1604:
	.cfi_startproc
	.cfi_personality 0x0,__gxx_personality_v0
	pushl	%ebp
	.cfi_def_cfa_offset 8
	movl	%esp, %ebp
	.cfi_offset 5, -8
	.cfi_def_cfa_register 5
#APP
# 27 "main.cpp" 1
	rdtsc
# 0 "" 2
#NO_APP
	popl	%ebp
	ret
	.cfi_endproc
.LFE1604:
	.size	_Z9__rdtsc__v, .-_Z9__rdtsc__v
	.p2align 4,,15
	.type	_GLOBAL__I__ZN5smath10del_memoryEPv, @function
_GLOBAL__I__ZN5smath10del_memoryEPv:
.LFB1651:
	.cfi_startproc
	.cfi_personality 0x0,__gxx_personality_v0
	pushl	%ebp
	.cfi_def_cfa_offset 8
	movl	%esp, %ebp
	.cfi_offset 5, -8
	.cfi_def_cfa_register 5
	subl	$24, %esp
	movl	$_ZStL8__ioinit, (%esp)
	call	_ZNSt8ios_base4InitC1Ev
	movl	$__dso_handle, 8(%esp)
	movl	$_ZStL8__ioinit, 4(%esp)
	movl	$_ZNSt8ios_base4InitD1Ev, (%esp)
	call	__cxa_atexit
	leave
	ret
	.cfi_endproc
.LFE1651:
	.size	_GLOBAL__I__ZN5smath10del_memoryEPv, .-_GLOBAL__I__ZN5smath10del_memoryEPv
	.section	.ctors,"aw",@progbits
	.align 4
	.long	_GLOBAL__I__ZN5smath10del_memoryEPv
	.text
	.p2align 4,,15
.globl _ZN5smath7ToColorERNS_6VectorIU8__vectorfLi1EEE
	.type	_ZN5smath7ToColorERNS_6VectorIU8__vectorfLi1EEE, @function
_ZN5smath7ToColorERNS_6VectorIU8__vectorfLi1EEE:
.LFB1590:
	.cfi_startproc
	.cfi_personality 0x0,__gxx_personality_v0
	pushl	%ebp
	.cfi_def_cfa_offset 8
	movl	%esp, %ebp
	.cfi_offset 5, -8
	.cfi_def_cfa_register 5
	subl	$32, %esp
	movl	8(%ebp), %eax
	movaps	.LC0, %xmm1
	movaps	(%eax), %xmm0
	mulps	%xmm1, %xmm0
	minps	%xmm1, %xmm0
	movaps	%xmm0, (%eax)
	fnstcw	-18(%ebp)
	movups	%xmm0, -16(%ebp)
	movzwl	-18(%ebp), %eax
	flds	-16(%ebp)
	movb	$12, %ah
	movw	%ax, -20(%ebp)
	fldcw	-20(%ebp)
	fistpll	-32(%ebp)
	fldcw	-18(%ebp)
	movl	-32(%ebp), %eax
	flds	-12(%ebp)
	fldcw	-20(%ebp)
	fistpll	-32(%ebp)
	fldcw	-18(%ebp)
	movl	-32(%ebp), %edx
	sall	$8, %eax
	flds	-8(%ebp)
	addl	%edx, %eax
	fldcw	-20(%ebp)
	fistpll	-32(%ebp)
	fldcw	-18(%ebp)
	movl	-32(%ebp), %edx
	sall	$8, %eax
	leave
	addl	%edx, %eax
	ret
	.cfi_endproc
.LFE1590:
	.size	_ZN5smath7ToColorERNS_6VectorIU8__vectorfLi1EEE, .-_ZN5smath7ToColorERNS_6VectorIU8__vectorfLi1EEE
	.p2align 4,,15
.globl _ZN5smath5crossERKNS_6VectorIU8__vectorfLi1EEES4_
	.type	_ZN5smath5crossERKNS_6VectorIU8__vectorfLi1EEES4_, @function
_ZN5smath5crossERKNS_6VectorIU8__vectorfLi1EEES4_:
.LFB1589:
	.cfi_startproc
	.cfi_personality 0x0,__gxx_personality_v0
	pushl	%ebp
	.cfi_def_cfa_offset 8
	movl	%esp, %ebp
	.cfi_offset 5, -8
	.cfi_def_cfa_register 5
	movl	12(%ebp), %edx
	movl	8(%ebp), %eax
	movaps	(%edx), %xmm2
	movl	16(%ebp), %edx
	movaps	%xmm2, %xmm0
	shufps	$201, %xmm2, %xmm0
	shufps	$210, %xmm2, %xmm2
	movaps	(%edx), %xmm1
	movaps	%xmm1, %xmm3
	shufps	$210, %xmm1, %xmm3
	shufps	$201, %xmm1, %xmm1
	mulps	%xmm3, %xmm0
	mulps	%xmm1, %xmm2
	subps	%xmm2, %xmm0
	movaps	%xmm0, (%eax)
	popl	%ebp
	ret	$4
	.cfi_endproc
.LFE1589:
	.size	_ZN5smath5crossERKNS_6VectorIU8__vectorfLi1EEES4_, .-_ZN5smath5crossERKNS_6VectorIU8__vectorfLi1EEES4_
	.p2align 4,,15
.globl _ZN5smath10del_memoryEPv
	.type	_ZN5smath10del_memoryEPv, @function
_ZN5smath10del_memoryEPv:
.LFB1493:
	.cfi_startproc
	.cfi_personality 0x0,__gxx_personality_v0
	pushl	%ebp
	.cfi_def_cfa_offset 8
	movl	%esp, %ebp
	.cfi_offset 5, -8
	.cfi_def_cfa_register 5
	subl	$8, %esp
	leave
	jmp	free
	.cfi_endproc
.LFE1493:
	.size	_ZN5smath10del_memoryEPv, .-_ZN5smath10del_memoryEPv
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC5:
	.string	"\ntime: "
.LC6:
	.string	"\nresult: "
.LC7:
	.string	"\n"
	.text
	.p2align 4,,15
.globl main
	.type	main, @function
main:
.LFB1605:
	.cfi_startproc
	.cfi_personality 0x0,__gxx_personality_v0
	pushl	%ebp
	.cfi_def_cfa_offset 8
	movl	%esp, %ebp
	.cfi_offset 5, -8
	.cfi_def_cfa_register 5
	andl	$-16, %esp
	subl	$96, %esp
	movaps	.LC2, %xmm0
#APP
# 27 "main.cpp" 1
	rdtsc
# 0 "" 2
#NO_APP
	movups	%xmm0, 80(%esp)
	addps	.LC4, %xmm0
	movl	%eax, 72(%esp)
	flds	80(%esp)
	fadds	84(%esp)
	movl	%edx, 76(%esp)
	fadds	88(%esp)
	fadds	92(%esp)
	fldz
	fadd	%st, %st(1)
	fxch	%st(1)
	fstps	28(%esp)
	movss	28(%esp), %xmm2
	shufps	$0, %xmm2, %xmm2
	movups	%xmm2, 80(%esp)
	flds	80(%esp)
	fadds	84(%esp)
	fadds	88(%esp)
	fadds	92(%esp)
	movups	%xmm0, 80(%esp)
	fadd	%st(1), %st
	flds	80(%esp)
	fadds	84(%esp)
	fadds	88(%esp)
	fadds	92(%esp)
	fadd	%st(2), %st
	fxch	%st(1)
	fstps	28(%esp)
	movss	28(%esp), %xmm0
	fstps	28(%esp)
	movss	28(%esp), %xmm2
	shufps	$0, %xmm0, %xmm0
	shufps	$0, %xmm2, %xmm2
	movaps	%xmm0, %xmm1
	movaps	%xmm2, %xmm0
	mulps	%xmm1, %xmm0
	movups	%xmm0, 80(%esp)
	flds	80(%esp)
	fadds	84(%esp)
	fadds	88(%esp)
	fadds	92(%esp)
	faddp	%st, %st(1)
	fadd	%st(0), %st
#APP
# 27 "main.cpp" 1
	rdtsc
# 0 "" 2
#NO_APP
	movl	%eax, 16(%esp)
	movl	%edx, 20(%esp)
	fstps	48(%esp)
	movl	$7, 8(%esp)
	movl	$.LC5, 4(%esp)
	movl	$_ZSt4cout, (%esp)
	call	_ZSt16__ostream_insertIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_PKS3_i
	movl	16(%esp), %edx
	movl	20(%esp), %ecx
	subl	72(%esp), %edx
	sbbl	76(%esp), %ecx
	movl	$_ZSt4cout, (%esp)
	movl	%edx, 4(%esp)
	movl	%ecx, 8(%esp)
	call	_ZNSo9_M_insertIyEERSoT_
	movl	$9, 8(%esp)
	movl	$.LC6, 4(%esp)
	movl	$_ZSt4cout, (%esp)
	call	_ZSt16__ostream_insertIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_PKS3_i
	movl	$_ZSt4cout, (%esp)
	flds	48(%esp)
	fstpl	4(%esp)
	call	_ZNSo9_M_insertIdEERSoT_
	movl	$1, 8(%esp)
	movl	$.LC7, 4(%esp)
	movl	$_ZSt4cout, (%esp)
	call	_ZSt16__ostream_insertIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_PKS3_i
	xorl	%eax, %eax
	leave
	ret
	.cfi_endproc
.LFE1605:
	.size	main, .-main
	.local	_ZStL8__ioinit
	.comm	_ZStL8__ioinit,1,1
	.weakref	_ZL20__gthrw_pthread_oncePiPFvvE,pthread_once
	.weakref	_ZL27__gthrw_pthread_getspecificj,pthread_getspecific
	.weakref	_ZL27__gthrw_pthread_setspecificjPKv,pthread_setspecific
	.weakref	_ZL22__gthrw_pthread_createPmPK14pthread_attr_tPFPvS3_ES3_,pthread_create
	.weakref	_ZL20__gthrw_pthread_joinmPPv,pthread_join
	.weakref	_ZL21__gthrw_pthread_equalmm,pthread_equal
	.weakref	_ZL20__gthrw_pthread_selfv,pthread_self
	.weakref	_ZL22__gthrw_pthread_detachm,pthread_detach
	.weakref	_ZL22__gthrw_pthread_cancelm,pthread_cancel
	.weakref	_ZL19__gthrw_sched_yieldv,sched_yield
	.weakref	_ZL26__gthrw_pthread_mutex_lockP15pthread_mutex_t,pthread_mutex_lock
	.weakref	_ZL29__gthrw_pthread_mutex_trylockP15pthread_mutex_t,pthread_mutex_trylock
	.weakref	_ZL31__gthrw_pthread_mutex_timedlockP15pthread_mutex_tPK8timespec,pthread_mutex_timedlock
	.weakref	_ZL28__gthrw_pthread_mutex_unlockP15pthread_mutex_t,pthread_mutex_unlock
	.weakref	_ZL26__gthrw_pthread_mutex_initP15pthread_mutex_tPK19pthread_mutexattr_t,pthread_mutex_init
	.weakref	_ZL29__gthrw_pthread_mutex_destroyP15pthread_mutex_t,pthread_mutex_destroy
	.weakref	_ZL30__gthrw_pthread_cond_broadcastP14pthread_cond_t,pthread_cond_broadcast
	.weakref	_ZL27__gthrw_pthread_cond_signalP14pthread_cond_t,pthread_cond_signal
	.weakref	_ZL25__gthrw_pthread_cond_waitP14pthread_cond_tP15pthread_mutex_t,pthread_cond_wait
	.weakref	_ZL30__gthrw_pthread_cond_timedwaitP14pthread_cond_tP15pthread_mutex_tPK8timespec,pthread_cond_timedwait
	.weakref	_ZL28__gthrw_pthread_cond_destroyP14pthread_cond_t,pthread_cond_destroy
	.weakref	_ZL26__gthrw_pthread_key_createPjPFvPvE,pthread_key_create
	.weakref	_ZL26__gthrw_pthread_key_deletej,pthread_key_delete
	.weakref	_ZL30__gthrw_pthread_mutexattr_initP19pthread_mutexattr_t,pthread_mutexattr_init
	.weakref	_ZL33__gthrw_pthread_mutexattr_settypeP19pthread_mutexattr_ti,pthread_mutexattr_settype
	.weakref	_ZL33__gthrw_pthread_mutexattr_destroyP19pthread_mutexattr_t,pthread_mutexattr_destroy
	.section	.rodata.cst16,"aM",@progbits,16
	.align 16
.LC0:
	.long	1132396544
	.long	1132396544
	.long	1132396544
	.long	1132396544
	.align 16
.LC2:
	.long	1065353216
	.long	1065353216
	.long	1065353216
	.long	1065353216
	.align 16
.LC4:
	.long	1056964608
	.long	1056964608
	.long	1056964608
	.long	1056964608
	.ident	"GCC: (SUSE Linux) 4.4.1 [gcc-4_4-branch revision 150839]"
	.section	.comment.SUSE.OPTs,"MS",@progbits,1
	.string	"Ospwg"
	.section	.note.GNU-stack,"",@progbits
