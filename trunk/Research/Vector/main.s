# -- Machine type PW
# mark_description "Intel(R) C++ Compiler for applications running on IA-32, Version 11.1    Build 20091012 %s";
# mark_description "-long_double -O2 -msse2 -fpermissive -S";
	.file "main.cpp"
	.section .ctors, "wa"
	.text
..TXTST0:
# -- Begin  main
# mark_begin;
       .align    16,0x90
	.globl main
main:
# parameter 1: 8 + %ebp
# parameter 2: 12 + %ebp
..B1.1:                         # Preds ..B1.0
..___tag_value_main.1:                                          #34.1
        pushl     %ebp                                          #34.1
..___tag_value_main.2:                                          #
        movl      %esp, %ebp                                    #34.1
..___tag_value_main.3:                                          #
        andl      $-128, %esp                                   #34.1
        pushl     %edi                                          #34.1
..___tag_value_main.5:                                          #
        pushl     %esi                                          #34.1
..___tag_value_main.6:                                          #
        pushl     %ebx                                          #34.1
..___tag_value_main.7:                                          #
        subl      $244, %esp                                    #34.1
        pushl     $3                                            #34.1
..___tag_value_main.8:                                          #34.1
        call      __intel_new_proc_init                         #34.1
..___tag_value_main.9:                                          #
                                # LOE
..B1.9:                         # Preds ..B1.1
        movaps    _2il0floatpacket.307, %xmm1                   #42.4
        movaps    %xmm1, %xmm3                                  #74.56
        mulps     %xmm1, %xmm3                                  #74.56
        movaps    %xmm3, %xmm4                                  #74.56
        stmxcsr   196(%esp)                                     #34.1
        orl       $32768, 196(%esp)                             #34.1
        ldmxcsr   196(%esp)                                     #34.1
        rdtsc                                                   #72.7
        movaps    _2il0floatpacket.308, %xmm0                   #74.22
        movups    %xmm3, 148(%esp)                              #74.56
        addss     152(%esp), %xmm4                              #74.56
        mulps     %xmm1, %xmm0                                  #74.22
        movl      %eax, 204(%esp)                               #72.7
        movl      %edx, %ebx                                    #72.7
        addps     %xmm1, %xmm0                                  #74.22
        mulps     %xmm1, %xmm0                                  #74.22
        movups    %xmm0, 132(%esp)                              #74.22
        unpckhps  %xmm3, %xmm3                                  #74.56
        movaps    %xmm0, %xmm2                                  #74.22
        addss     136(%esp), %xmm2                              #74.22
        unpckhps  %xmm0, %xmm0                                  #74.22
        addss     %xmm2, %xmm0                                  #74.22
        addss     144(%esp), %xmm0                              #74.22
        addss     %xmm4, %xmm3                                  #74.56
        addss     160(%esp), %xmm3                              #74.56
        shufps    $0, %xmm0, %xmm0                              #74.15
        mulps     %xmm1, %xmm0                                  #74.15
        shufps    $0, %xmm3, %xmm3                              #74.47
        mulps     %xmm1, %xmm3                                  #74.47
        mulps     %xmm1, %xmm3                                  #74.47
        movups    %xmm3, 164(%esp)                              #74.47
        movaps    %xmm3, %xmm5                                  #74.47
        addss     168(%esp), %xmm5                              #74.47
        unpckhps  %xmm3, %xmm3                                  #74.47
        addss     %xmm5, %xmm3                                  #74.47
        addss     176(%esp), %xmm3                              #74.47
        shufps    $0, %xmm3, %xmm3                              #74.15
        mulps     %xmm3, %xmm1                                  #74.15
        mulps     %xmm1, %xmm0                                  #74.15
        movups    %xmm0, 180(%esp)                              #74.15
        movaps    %xmm0, %xmm6                                  #74.15
        addss     184(%esp), %xmm6                              #74.15
        unpckhps  %xmm0, %xmm0                                  #74.15
        addss     %xmm6, %xmm0                                  #74.15
        addss     192(%esp), %xmm0                              #74.15
        addss     %xmm0, %xmm0                                  #74.15
        movss     %xmm0, 200(%esp)                              #74.15
        pushl     $_2__STRING.0                                 #78.7
        pushl     $_ZSt4cout                                    #78.7
        rdtsc                                                   #76.7
        movl      %eax, %esi                                    #76.7
        movl      %edx, %edi                                    #76.7
..___tag_value_main.10:                                         #78.7
        call      _ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc #78.7
..___tag_value_main.11:                                         #
                                # LOE eax ebx esi edi
..B1.2:                         # Preds ..B1.9
        subl      212(%esp), %esi                               #76.7
        sbbl      %ebx, %edi                                    #76.7
        pushl     %edi                                          #78.21
        pushl     %esi                                          #78.21
        pushl     %eax                                          #78.21
..___tag_value_main.12:                                         #78.21
        call      _ZNSolsEy                                     #78.21
..___tag_value_main.13:                                         #
                                # LOE
..B1.3:                         # Preds ..B1.2
        pushl     $_2__STRING.1                                 #79.7
        pushl     $_ZSt4cout                                    #79.7
..___tag_value_main.14:                                         #79.7
        call      _ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc #79.7
..___tag_value_main.15:                                         #
                                # LOE eax
..B1.11:                        # Preds ..B1.3
        addl      $32, %esp                                     #79.7
                                # LOE eax
..B1.4:                         # Preds ..B1.11
        movss     196(%esp), %xmm0                              #74.2
        movss     %xmm0, 4(%esp)                                #74.2
        movl      %eax, (%esp)                                  #79.7
..___tag_value_main.16:                                         #79.23
        call      _ZNSolsEf                                     #79.23
..___tag_value_main.17:                                         #
                                # LOE
..B1.5:                         # Preds ..B1.4
        pushl     $_2__STRING.2                                 #80.7
        pushl     $_ZSt4cout                                    #80.7
..___tag_value_main.18:                                         #80.7
        call      _ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc #80.7
..___tag_value_main.19:                                         #
                                # LOE
..B1.6:                         # Preds ..B1.5
        xorl      %eax, %eax                                    #173.9
        addl      $252, %esp                                    #173.9
..___tag_value_main.20:                                         #173.9
        popl      %ebx                                          #173.9
..___tag_value_main.21:                                         #173.9
        popl      %esi                                          #173.9
..___tag_value_main.22:                                         #173.9
        popl      %edi                                          #173.9
        movl      %ebp, %esp                                    #173.9
        popl      %ebp                                          #173.9
..___tag_value_main.23:                                         #
        ret                                                     #173.9
        .align    16,0x90
..___tag_value_main.25:                                         #
                                # LOE
# mark_end;
	.type	main,@function
	.size	main,.-main
	.data
# -- End  main
	.text
# -- Begin  __sti__$E
# mark_begin;
       .align    16,0x90
__sti__$E:
..B2.1:                         # Preds ..B2.0
..___tag_value___sti__$E.26:                                    #
        pushl     $_ZSt8__ioinit                                #72.25
..___tag_value___sti__$E.27:                                    #
        call      _ZNSt8ios_base4InitC1Ev                       #72.25
..___tag_value___sti__$E.29:                                    #
                                # LOE ebx ebp esi edi
..B2.2:                         # Preds ..B2.1
        pushl     $__dso_handle                                 #72.25
..___tag_value___sti__$E.30:                                    #
        pushl     $_ZSt8__ioinit                                #72.25
..___tag_value___sti__$E.31:                                    #
        pushl     $_ZNSt8ios_base4InitD1Ev                      #72.25
..___tag_value___sti__$E.32:                                    #
        call      __cxa_atexit                                  #72.25
..___tag_value___sti__$E.34:                                    #
                                # LOE ebx ebp esi edi
..B2.6:                         # Preds ..B2.2
        addl      $16, %esp                                     #72.25
..___tag_value___sti__$E.35:                                    #
                                # LOE ebx ebp esi edi
..B2.3:                         # Preds ..B2.6
        ret                                                     #72.25
        .align    16,0x90
..___tag_value___sti__$E.36:                                    #
                                # LOE
# mark_end;
	.type	__sti__$E,@function
	.size	__sti__$E,.-__sti__$E
	.data
# -- End  __sti__$E
	.bss
	.align 4
	.align 1
_ZSt8__ioinit:
	.type	_ZSt8__ioinit,@object
	.size	_ZSt8__ioinit,1
	.space 1	# pad
	.section .rodata, "a"
	.align 16
	.align 16
_2il0floatpacket.307:
	.long	0x3f800000,0x3f800000,0x3f800000,0x3f800000
	.type	_2il0floatpacket.307,@object
	.size	_2il0floatpacket.307,16
	.align 16
_2il0floatpacket.308:
	.long	0x3f000000,0x3f000000,0x3f000000,0x3f000000
	.type	_2il0floatpacket.308,@object
	.size	_2il0floatpacket.308,16
	.section .rodata.str1.4, "aMS",@progbits,1
	.align 4
	.align 4
_2__STRING.0:
	.byte	10
	.byte	116
	.byte	105
	.byte	109
	.byte	101
	.byte	58
	.byte	32
	.byte	0
	.type	_2__STRING.0,@object
	.size	_2__STRING.0,8
	.align 4
_2__STRING.1:
	.byte	10
	.byte	114
	.byte	101
	.byte	115
	.byte	117
	.byte	108
	.byte	116
	.byte	58
	.byte	32
	.byte	0
	.type	_2__STRING.1,@object
	.size	_2__STRING.1,10
	.space 2	# pad
	.align 4
_2__STRING.2:
	.byte	10
	.byte	0
	.type	_2__STRING.2,@object
	.size	_2__STRING.2,2
	.section .ctors, "wa"
	.align 4
__init_0:
	.type	__init_0,@object
	.size	__init_0,4
	.long	__sti__$E
	.data
	.hidden __dso_handle
# mark_proc_addr_taken __sti__$E;
# mark_proc_addr_taken _ZNSt8ios_base4InitD1Ev;
	.weak pthread_setspecific
	.weak pthread_getspecific
	.weak pthread_key_delete
	.weak pthread_key_create
	.weak pthread_cond_timedwait
	.weak pthread_cond_wait
	.weak pthread_cond_broadcast
	.weak pthread_cond_signal
	.weak pthread_cond_destroy
	.weak pthread_mutexattr_settype
	.weak pthread_mutexattr_destroy
	.weak pthread_mutexattr_init
	.weak pthread_mutex_unlock
	.weak pthread_mutex_timedlock
	.weak pthread_mutex_lock
	.weak pthread_mutex_trylock
	.weak pthread_mutex_destroy
	.weak pthread_mutex_init
	.weak pthread_cancel
	.weak pthread_once
	.weak pthread_equal
	.weak pthread_self
	.weak pthread_detach
	.weak pthread_join
	.weak pthread_create
	.weak sched_yield
	.section .note.GNU-stack, ""
// -- Begin DWARF2 SEGMENT .debug_line
	.section .debug_line
.debug_line_seg:
	.align 1
// -- Begin DWARF2 SEGMENT .eh_frame
	.section .eh_frame,"a",@progbits
.eh_frame_seg:
	.align 1
	.4byte 0x00000018
	.4byte 0x00000000
	.byte 0x01
	.4byte 0x004c507a
	.byte 0x01
	.byte 0x7c
	.byte 0x08
	.byte 0x06
	.byte 0x00
	.4byte __gxx_personality_v0
	.byte 0x00
	.4byte 0x8804040c
	.byte 0x01
	.4byte 0x000000b4
	.4byte 0x00000020
	.4byte ..___tag_value_main.1
	.4byte ..___tag_value_main.25-..___tag_value_main.1
	.byte 0x04
	.4byte 0x00000000
	.byte 0x04
	.4byte ..___tag_value_main.2-..___tag_value_main.1
	.byte 0x0c
	.2byte 0x0804
	.byte 0x04
	.4byte ..___tag_value_main.3-..___tag_value_main.2
	.byte 0x0c
	.2byte 0x0805
	.byte 0x85
	.byte 0x02
	.byte 0x04
	.4byte ..___tag_value_main.5-..___tag_value_main.3
	.byte 0x10
	.8byte 0xffff800d1c380e07
	.8byte 0x22fffffffc0d1aff
	.byte 0x04
	.4byte ..___tag_value_main.6-..___tag_value_main.5
	.byte 0x10
	.8byte 0xffff800d1c380e06
	.8byte 0x22fffffff80d1aff
	.byte 0x04
	.4byte ..___tag_value_main.7-..___tag_value_main.6
	.byte 0x10
	.8byte 0xffff800d1c380e03
	.8byte 0x22fffffff40d1aff
	.byte 0x04
	.4byte ..___tag_value_main.8-..___tag_value_main.7
	.byte 0x2e
	.byte 0x04
	.byte 0x04
	.4byte ..___tag_value_main.10-..___tag_value_main.8
	.byte 0x2e
	.byte 0x0c
	.byte 0x04
	.4byte ..___tag_value_main.12-..___tag_value_main.10
	.byte 0x2e
	.byte 0x18
	.byte 0x04
	.4byte ..___tag_value_main.14-..___tag_value_main.12
	.byte 0x2e
	.byte 0x20
	.byte 0x04
	.4byte ..___tag_value_main.16-..___tag_value_main.14
	.byte 0x2e
	.byte 0x00
	.byte 0x04
	.4byte ..___tag_value_main.18-..___tag_value_main.16
	.byte 0x2e
	.byte 0x08
	.byte 0x04
	.4byte ..___tag_value_main.20-..___tag_value_main.18
	.byte 0x09
	.2byte 0x0303
	.byte 0x04
	.4byte ..___tag_value_main.21-..___tag_value_main.20
	.byte 0x09
	.2byte 0x0606
	.byte 0x04
	.4byte ..___tag_value_main.22-..___tag_value_main.21
	.byte 0x09
	.2byte 0x0707
	.byte 0x04
	.4byte ..___tag_value_main.23-..___tag_value_main.22
	.byte 0x0c
	.2byte 0x0404
	.byte 0x09
	.2byte 0x0505
	.2byte 0x0000
	.4byte 0x00000040
	.4byte 0x000000d8
	.4byte ..___tag_value___sti__$E.26
	.4byte ..___tag_value___sti__$E.36-..___tag_value___sti__$E.26
	.byte 0x04
	.4byte 0x00000000
	.byte 0x04
	.4byte ..___tag_value___sti__$E.27-..___tag_value___sti__$E.26
	.byte 0x0c
	.2byte 0x0804
	.byte 0x2e
	.byte 0x04
	.byte 0x04
	.4byte ..___tag_value___sti__$E.30-..___tag_value___sti__$E.27
	.byte 0x0c
	.2byte 0x0c04
	.byte 0x04
	.4byte ..___tag_value___sti__$E.31-..___tag_value___sti__$E.30
	.byte 0x0c
	.2byte 0x1004
	.byte 0x04
	.4byte ..___tag_value___sti__$E.32-..___tag_value___sti__$E.31
	.byte 0x0c
	.2byte 0x1404
	.byte 0x2e
	.byte 0x10
	.byte 0x04
	.4byte ..___tag_value___sti__$E.35-..___tag_value___sti__$E.32
	.byte 0x0c
	.2byte 0x0404
	.2byte 0x0000
	.byte 0x00
# End
