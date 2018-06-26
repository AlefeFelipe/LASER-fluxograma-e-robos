	.file	"main.cpp"
	.section	.rodata
.LC1:
	.string	"fluxprog"
.LC2:
	.string	"images/menu.png"
.LC3:
	.string	"images/seta.png"
.LC4:
	.string	"images/voltita.png"
.LC5:
	.string	"images/ultrassom.png"
.LC6:
	.string	"images/logic_true.png"
.LC7:
	.string	"images/logic_false.png"
.LC8:
	.string	"images/fita.png"
.LC9:
	.string	"images/point.png"
.LC10:
	.string	"images/hpoint.png"
.LC11:
	.string	"images/block1.png"
.LC12:
	.string	"images/hblock1.png"
.LC13:
	.string	"images/sblock1.png"
.LC14:
	.string	"images/pblock1.png"
.LC15:
	.string	"images/block2.png"
.LC16:
	.string	"images/hblock2.png"
.LC17:
	.string	"images/sblock2.png"
.LC18:
	.string	"images/pblock2.png"
.LC19:
	.string	"images/block3.png"
.LC20:
	.string	"images/hblock3.png"
.LC21:
	.string	"images/sblock3.png"
.LC22:
	.string	"images/pblock3.png"
.LC23:
	.string	"images/block4.png"
.LC24:
	.string	"images/hblock4.png"
.LC25:
	.string	"images/sblock4.png"
.LC26:
	.string	"images/pblock4.png"
.LC27:
	.string	"images/block5.png"
.LC28:
	.string	"images/hblock5.png"
.LC29:
	.string	"images/sblock5.png"
.LC30:
	.string	"images/pblock5.png"
.LC31:
	.string	"images/NUMBER_0.png"
.LC32:
	.string	"images/NUMBER_1.png"
.LC33:
	.string	"images/NUMBER_2.png"
.LC34:
	.string	"images/NUMBER_3.png"
.LC35:
	.string	"images/NUMBER_4.png"
.LC36:
	.string	"images/NUMBER_5.png"
.LC37:
	.string	"images/NUMBER_6.png"
.LC38:
	.string	"images/NUMBER_7.png"
.LC39:
	.string	"images/NUMBER_8.png"
.LC40:
	.string	"images/NUMBER_9.png"
	.text
	.globl	main
	.type	main, @function
main:
.LFB17:
	.cfi_startproc
	.cfi_personality 0x3,__gxx_personality_v0
	.cfi_lsda 0x3,.LLSDA17
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	pushq	%r12
	pushq	%rbx
	subq	$176, %rsp
	.cfi_offset 12, -24
	.cfi_offset 3, -32
	movl	%edi, -164(%rbp)
	movq	%rsi, -176(%rbp)
	movq	%fs:40, %rax
	movq	%rax, -24(%rbp)
	xorl	%eax, %eax
	movl	$atexit, %esi
	movl	$84017665, %edi
.LEHB0:
	call	al_install_system
	call	al_init_image_addon
	call	al_init_primitives_addon
	call	al_install_keyboard
	call	al_install_mouse
	movl	$0, dmouse_x(%rip)
	movl	$0, dmouse_y(%rip)
	movl	$0, current_mouse_wheel(%rip)
	movl	$0, start_block(%rip)
	movl	$-1, mode(%rip)
	movss	.LC0(%rip), %xmm0
	movss	%xmm0, scale(%rip)
	movb	$0, press_mouse(%rip)
	movb	$0, press_mouse+1(%rip)
	movb	$0, press_mouse+2(%rip)
	movb	$0, hold_mouse(%rip)
	movb	$0, hold_mouse+1(%rip)
	movb	$0, hold_mouse+2(%rip)
	movb	$0, release_mouse(%rip)
	movb	$0, release_mouse+1(%rip)
	movb	$0, release_mouse+2(%rip)
	movl	$600, %esi
	movl	$800, %edi
	call	al_create_display
	movq	%rax, screen(%rip)
	movq	screen(%rip), %rax
	movl	$1, %esi
	movq	%rax, %rdi
	call	al_set_system_mouse_cursor
	movq	screen(%rip), %rax
	movl	$.LC1, %esi
	movq	%rax, %rdi
	call	al_set_window_title
	movl	$.LC2, %edi
	call	al_load_bitmap
	movq	%rax, -136(%rbp)
	movl	$.LC3, %edi
	call	al_load_bitmap
	movq	%rax, seta(%rip)
	movl	$.LC4, %edi
	call	al_load_bitmap
	movq	%rax, voltita(%rip)
	movl	$.LC5, %edi
	call	al_load_bitmap
	movq	%rax, ultrassom(%rip)
	movl	$.LC6, %edi
	call	al_load_bitmap
	movq	%rax, logic_true(%rip)
	movl	$.LC7, %edi
	call	al_load_bitmap
	movq	%rax, logic_false(%rip)
	movl	$.LC8, %edi
	call	al_load_bitmap
	movq	%rax, fita(%rip)
	movl	$.LC9, %edi
	call	al_load_bitmap
	movq	%rax, CON_POINT(%rip)
	movl	$.LC10, %edi
	call	al_load_bitmap
	movq	%rax, CON_POINT+8(%rip)
	movl	$.LC11, %edi
	call	al_load_bitmap
	movq	%rax, BLOCKTYPE(%rip)
	movl	$.LC12, %edi
	call	al_load_bitmap
	movq	%rax, BLOCKTYPE+8(%rip)
	movl	$.LC13, %edi
	call	al_load_bitmap
	movq	%rax, BLOCKTYPE+16(%rip)
	movl	$.LC14, %edi
	call	al_load_bitmap
	movq	%rax, BLOCKTYPE+24(%rip)
	movl	$.LC15, %edi
	call	al_load_bitmap
	movq	%rax, BLOCKTYPE+32(%rip)
	movl	$.LC16, %edi
	call	al_load_bitmap
	movq	%rax, BLOCKTYPE+40(%rip)
	movl	$.LC17, %edi
	call	al_load_bitmap
	movq	%rax, BLOCKTYPE+48(%rip)
	movl	$.LC18, %edi
	call	al_load_bitmap
	movq	%rax, BLOCKTYPE+56(%rip)
	movl	$.LC19, %edi
	call	al_load_bitmap
	movq	%rax, BLOCKTYPE+64(%rip)
	movl	$.LC20, %edi
	call	al_load_bitmap
	movq	%rax, BLOCKTYPE+72(%rip)
	movl	$.LC21, %edi
	call	al_load_bitmap
	movq	%rax, BLOCKTYPE+80(%rip)
	movl	$.LC22, %edi
	call	al_load_bitmap
	movq	%rax, BLOCKTYPE+88(%rip)
	movl	$.LC23, %edi
	call	al_load_bitmap
	movq	%rax, BLOCKTYPE+96(%rip)
	movl	$.LC24, %edi
	call	al_load_bitmap
	movq	%rax, BLOCKTYPE+104(%rip)
	movl	$.LC25, %edi
	call	al_load_bitmap
	movq	%rax, BLOCKTYPE+112(%rip)
	movl	$.LC26, %edi
	call	al_load_bitmap
	movq	%rax, BLOCKTYPE+120(%rip)
	movl	$.LC27, %edi
	call	al_load_bitmap
	movq	%rax, BLOCKTYPE+128(%rip)
	movl	$.LC28, %edi
	call	al_load_bitmap
	movq	%rax, BLOCKTYPE+136(%rip)
	movl	$.LC29, %edi
	call	al_load_bitmap
	movq	%rax, BLOCKTYPE+144(%rip)
	movl	$.LC30, %edi
	call	al_load_bitmap
	movq	%rax, BLOCKTYPE+152(%rip)
	movl	$.LC31, %edi
	call	al_load_bitmap
	movq	%rax, NUMBER(%rip)
	movl	$.LC32, %edi
	call	al_load_bitmap
	movq	%rax, NUMBER+8(%rip)
	movl	$.LC33, %edi
	call	al_load_bitmap
	movq	%rax, NUMBER+16(%rip)
	movl	$.LC34, %edi
	call	al_load_bitmap
	movq	%rax, NUMBER+24(%rip)
	movl	$.LC35, %edi
	call	al_load_bitmap
	movq	%rax, NUMBER+32(%rip)
	movl	$.LC36, %edi
	call	al_load_bitmap
	movq	%rax, NUMBER+40(%rip)
	movl	$.LC37, %edi
	call	al_load_bitmap
	movq	%rax, NUMBER+48(%rip)
	movl	$.LC38, %edi
	call	al_load_bitmap
	movq	%rax, NUMBER+56(%rip)
	movl	$.LC39, %edi
	call	al_load_bitmap
	movq	%rax, NUMBER+64(%rip)
	movl	$.LC40, %edi
	call	al_load_bitmap
	movq	%rax, NUMBER+72(%rip)
	call	al_create_event_queue
	movq	%rax, -128(%rbp)
	call	al_create_event_queue
	movq	%rax, -120(%rbp)
	call	al_create_event_queue
	movq	%rax, -112(%rbp)
	call	al_get_mouse_event_source
	movq	%rax, %rdx
	movq	-128(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	al_register_event_source
	movq	screen(%rip), %rax
	movq	%rax, %rdi
	call	al_get_display_event_source
	movq	%rax, %rdx
	movq	-120(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	al_register_event_source
	call	al_get_keyboard_event_source
	movq	%rax, %rdx
	movq	-112(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	al_register_event_source
	movl	$24, %edi
	call	_Znwm
.LEHE0:
	movq	%rax, %rbx
	movq	%rbx, %rdi
.LEHB1:
	call	_ZN10block_listC1Ev
.LEHE1:
	movq	%rbx, -104(%rbp)
	movb	$0, -146(%rbp)
	movb	$0, -145(%rbp)
.L68:
	cmpb	$0, -145(%rbp)
	jne	.L2
	movw	$0, roll_mouse(%rip)
	movl	$0, -144(%rbp)
	movl	$0, -140(%rbp)
.L6:
	cmpl	$2, -140(%rbp)
	jg	.L3
	movl	-140(%rbp), %eax
	cltq
	movzbl	press_mouse(%rax), %eax
	testb	%al, %al
	je	.L4
	movl	-140(%rbp), %eax
	cltq
	movb	$0, press_mouse(%rax)
	movl	-140(%rbp), %eax
	cltq
	movb	$1, hold_mouse(%rax)
.L4:
	movl	-140(%rbp), %eax
	cltq
	movzbl	release_mouse(%rax), %eax
	testb	%al, %al
	je	.L5
	movl	-140(%rbp), %eax
	cltq
	movb	$0, release_mouse(%rax)
.L5:
	addl	$1, -140(%rbp)
	jmp	.L6
.L3:
	movq	-120(%rbp), %rax
	movq	%rax, %rdi
.LEHB2:
	call	al_is_event_queue_empty
	xorl	$1, %eax
	testb	%al, %al
	je	.L7
	leaq	-96(%rbp), %rdx
	movq	-120(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	al_wait_for_event
	movl	-96(%rbp), %eax
	cmpl	$42, %eax
	jne	.L3
	movb	$1, -145(%rbp)
	jmp	.L3
.L7:
	movq	-128(%rbp), %rax
	movq	%rax, %rdi
	call	al_is_event_queue_empty
	xorl	$1, %eax
	testb	%al, %al
	je	.L9
	leaq	-96(%rbp), %rdx
	movq	-128(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	al_wait_for_event
	movl	-96(%rbp), %eax
	cmpl	$22, %eax
	jne	.L10
	movl	-32(%rbp), %eax
	cmpl	$1, %eax
	jne	.L11
	movb	$1, release_mouse(%rip)
	movb	$0, hold_mouse(%rip)
.L11:
	movl	-32(%rbp), %eax
	cmpl	$2, %eax
	jne	.L12
	movb	$1, release_mouse+1(%rip)
	movb	$0, hold_mouse+1(%rip)
.L12:
	movl	-32(%rbp), %eax
	cmpl	$3, %eax
	jne	.L10
	movb	$1, release_mouse+2(%rip)
	movb	$0, hold_mouse+2(%rip)
.L10:
	movl	-96(%rbp), %eax
	cmpl	$21, %eax
	jne	.L13
	movl	-32(%rbp), %eax
	cmpl	$1, %eax
	jne	.L14
	movb	$1, press_mouse(%rip)
	movb	$0, hold_mouse(%rip)
.L14:
	movl	-32(%rbp), %eax
	cmpl	$2, %eax
	jne	.L15
	movb	$1, press_mouse+1(%rip)
	movb	$0, hold_mouse+1(%rip)
.L15:
	movl	-32(%rbp), %eax
	cmpl	$3, %eax
	jne	.L13
	movb	$1, press_mouse+2(%rip)
	movb	$0, hold_mouse+2(%rip)
.L13:
	movl	-96(%rbp), %eax
	cmpl	$20, %eax
	jne	.L7
	movl	-64(%rbp), %eax
	movl	%eax, mouse_x(%rip)
	movl	-60(%rbp), %eax
	movl	%eax, mouse_y(%rip)
	movl	mode(%rip), %eax
	cmpl	$-1, %eax
	jne	.L17
	movl	-60(%rbp), %edx
	movl	-64(%rbp), %ecx
	movq	-104(%rbp), %rax
	movl	%ecx, %esi
	movq	%rax, %rdi
	call	_ZN10block_list15check_collisionEii
.L17:
	movl	-56(%rbp), %eax
	movl	current_mouse_wheel(%rip), %edx
	addl	$5, %edx
	cmpl	%edx, %eax
	jle	.L18
	movw	$1, roll_mouse(%rip)
	movl	-56(%rbp), %eax
	movl	%eax, current_mouse_wheel(%rip)
.L18:
	movl	-56(%rbp), %eax
	movl	current_mouse_wheel(%rip), %edx
	subl	$5, %edx
	cmpl	%edx, %eax
	jge	.L7
	movw	$-1, roll_mouse(%rip)
	movl	-56(%rbp), %eax
	movl	%eax, current_mouse_wheel(%rip)
	jmp	.L7
.L9:
	movq	-112(%rbp), %rax
	movq	%rax, %rdi
	call	al_is_event_queue_empty
	xorl	$1, %eax
	testb	%al, %al
	je	.L19
	leaq	-96(%rbp), %rdx
	movq	-112(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	al_wait_for_event
	movl	-96(%rbp), %eax
	cmpl	$10, %eax
	jne	.L9
	movl	-64(%rbp), %eax
	subl	$5, %eax
	cmpl	$54, %eax
	ja	.L9
	movl	%eax, %eax
	movq	.L23(,%rax,8), %rax
	jmp	*%rax
	.section	.rodata
	.align 8
	.align 4
.L23:
	.quad	.L22
	.quad	.L9
	.quad	.L9
	.quad	.L9
	.quad	.L9
	.quad	.L9
	.quad	.L9
	.quad	.L9
	.quad	.L9
	.quad	.L9
	.quad	.L9
	.quad	.L9
	.quad	.L24
	.quad	.L25
	.quad	.L9
	.quad	.L26
	.quad	.L9
	.quad	.L9
	.quad	.L27
	.quad	.L9
	.quad	.L28
	.quad	.L9
	.quad	.L9
	.quad	.L29
	.quad	.L30
	.quad	.L31
	.quad	.L32
	.quad	.L33
	.quad	.L9
	.quad	.L9
	.quad	.L9
	.quad	.L9
	.quad	.L9
	.quad	.L9
	.quad	.L9
	.quad	.L9
	.quad	.L9
	.quad	.L9
	.quad	.L9
	.quad	.L9
	.quad	.L9
	.quad	.L9
	.quad	.L9
	.quad	.L9
	.quad	.L9
	.quad	.L9
	.quad	.L9
	.quad	.L9
	.quad	.L9
	.quad	.L9
	.quad	.L9
	.quad	.L9
	.quad	.L9
	.quad	.L9
	.quad	.L34
	.text
.L34:
	movb	$1, -145(%rbp)
	jmp	.L20
.L29:
	movq	screen(%rip), %rax
	movq	%rax, %rdi
	call	al_get_display_height
	movl	%eax, %edx
	shrl	$31, %edx
	addl	%edx, %eax
	sarl	%eax
	movl	%eax, %ebx
	movq	screen(%rip), %rax
	movq	%rax, %rdi
	call	al_get_display_width
	movl	%eax, %edx
	shrl	$31, %edx
	addl	%edx, %eax
	sarl	%eax
	movl	%eax, %edx
	movq	-104(%rbp), %rax
	movl	$0, %r8d
	movl	%ebx, %ecx
	movl	$1, %esi
	movq	%rax, %rdi
	call	_ZN10block_list9new_blockEiiib
	jmp	.L20
.L30:
	movq	screen(%rip), %rax
	movq	%rax, %rdi
	call	al_get_display_height
	movl	%eax, %edx
	shrl	$31, %edx
	addl	%edx, %eax
	sarl	%eax
	movl	%eax, %ebx
	movq	screen(%rip), %rax
	movq	%rax, %rdi
	call	al_get_display_width
	movl	%eax, %edx
	shrl	$31, %edx
	addl	%edx, %eax
	sarl	%eax
	movl	%eax, %edx
	movq	-104(%rbp), %rax
	movl	$0, %r8d
	movl	%ebx, %ecx
	movl	$2, %esi
	movq	%rax, %rdi
	call	_ZN10block_list9new_blockEiiib
	jmp	.L20
.L31:
	movq	screen(%rip), %rax
	movq	%rax, %rdi
	call	al_get_display_height
	movl	%eax, %edx
	shrl	$31, %edx
	addl	%edx, %eax
	sarl	%eax
	movl	%eax, %ebx
	movq	screen(%rip), %rax
	movq	%rax, %rdi
	call	al_get_display_width
	movl	%eax, %edx
	shrl	$31, %edx
	addl	%edx, %eax
	sarl	%eax
	movl	%eax, %edx
	movq	-104(%rbp), %rax
	movl	$0, %r8d
	movl	%ebx, %ecx
	movl	$3, %esi
	movq	%rax, %rdi
	call	_ZN10block_list9new_blockEiiib
	jmp	.L20
.L32:
	movq	screen(%rip), %rax
	movq	%rax, %rdi
	call	al_get_display_height
	movl	%eax, %edx
	shrl	$31, %edx
	addl	%edx, %eax
	sarl	%eax
	movl	%eax, %ebx
	movq	screen(%rip), %rax
	movq	%rax, %rdi
	call	al_get_display_width
	movl	%eax, %edx
	shrl	$31, %edx
	addl	%edx, %eax
	sarl	%eax
	movl	%eax, %edx
	movq	-104(%rbp), %rax
	movl	$0, %r8d
	movl	%ebx, %ecx
	movl	$4, %esi
	movq	%rax, %rdi
	call	_ZN10block_list9new_blockEiiib
	jmp	.L20
.L33:
	movq	screen(%rip), %rax
	movq	%rax, %rdi
	call	al_get_display_height
	movl	%eax, %edx
	shrl	$31, %edx
	addl	%edx, %eax
	sarl	%eax
	movl	%eax, %ebx
	movq	screen(%rip), %rax
	movq	%rax, %rdi
	call	al_get_display_width
	movl	%eax, %edx
	shrl	$31, %edx
	addl	%edx, %eax
	sarl	%eax
	movl	%eax, %edx
	movq	-104(%rbp), %rax
	movl	$0, %r8d
	movl	%ebx, %ecx
	movl	$5, %esi
	movq	%rax, %rdi
	call	_ZN10block_list9new_blockEiiib
	jmp	.L20
.L24:
	movq	screen(%rip), %rax
	movq	%rax, %rdi
	call	al_get_display_height
	movl	%eax, %edx
	shrl	$31, %edx
	addl	%edx, %eax
	sarl	%eax
	movl	%eax, %ebx
	movq	screen(%rip), %rax
	movq	%rax, %rdi
	call	al_get_display_width
	movl	%eax, %edx
	shrl	$31, %edx
	addl	%edx, %eax
	sarl	%eax
	movl	%eax, %edx
	movq	-104(%rbp), %rax
	movl	$0, %r8d
	movl	%ebx, %ecx
	movl	$6, %esi
	movq	%rax, %rdi
	call	_ZN10block_list9new_blockEiiib
	jmp	.L20
.L27:
	movq	screen(%rip), %rax
	movq	%rax, %rdi
	call	al_get_display_height
	movl	%eax, %edx
	shrl	$31, %edx
	addl	%edx, %eax
	sarl	%eax
	movl	%eax, %ebx
	movq	screen(%rip), %rax
	movq	%rax, %rdi
	call	al_get_display_width
	movl	%eax, %edx
	shrl	$31, %edx
	addl	%edx, %eax
	sarl	%eax
	movl	%eax, %edx
	movq	-104(%rbp), %rax
	movl	$0, %r8d
	movl	%ebx, %ecx
	movl	$7, %esi
	movq	%rax, %rdi
	call	_ZN10block_list9new_blockEiiib
	jmp	.L20
.L22:
	movq	screen(%rip), %rax
	movq	%rax, %rdi
	call	al_get_display_height
	movl	%eax, %edx
	shrl	$31, %edx
	addl	%edx, %eax
	sarl	%eax
	movl	%eax, %ebx
	movq	screen(%rip), %rax
	movq	%rax, %rdi
	call	al_get_display_width
	movl	%eax, %edx
	shrl	$31, %edx
	addl	%edx, %eax
	sarl	%eax
	movl	%eax, %edx
	movq	-104(%rbp), %rax
	movl	$0, %r8d
	movl	%ebx, %ecx
	movl	$8, %esi
	movq	%rax, %rdi
	call	_ZN10block_list9new_blockEiiib
	jmp	.L20
.L25:
	movq	screen(%rip), %rax
	movq	%rax, %rdi
	call	al_get_display_height
	movl	%eax, %edx
	shrl	$31, %edx
	addl	%edx, %eax
	sarl	%eax
	movl	%eax, %ebx
	movq	screen(%rip), %rax
	movq	%rax, %rdi
	call	al_get_display_width
	movl	%eax, %edx
	shrl	$31, %edx
	addl	%edx, %eax
	sarl	%eax
	movl	%eax, %edx
	movq	-104(%rbp), %rax
	movl	$0, %r8d
	movl	%ebx, %ecx
	movl	$9, %esi
	movq	%rax, %rdi
	call	_ZN10block_list9new_blockEiiib
	jmp	.L20
.L26:
	movq	screen(%rip), %rax
	movq	%rax, %rdi
	call	al_get_display_height
	movl	%eax, %edx
	shrl	$31, %edx
	addl	%edx, %eax
	sarl	%eax
	movl	%eax, %ebx
	movq	screen(%rip), %rax
	movq	%rax, %rdi
	call	al_get_display_width
	movl	%eax, %edx
	shrl	$31, %edx
	addl	%edx, %eax
	sarl	%eax
	movl	%eax, %edx
	movq	-104(%rbp), %rax
	movl	$0, %r8d
	movl	%ebx, %ecx
	movl	$10, %esi
	movq	%rax, %rdi
	call	_ZN10block_list9new_blockEiiib
	jmp	.L20
.L28:
	movq	screen(%rip), %rax
	movq	%rax, %rdi
	call	al_get_display_height
	movl	%eax, %edx
	shrl	$31, %edx
	addl	%edx, %eax
	sarl	%eax
	movl	%eax, %ebx
	movq	screen(%rip), %rax
	movq	%rax, %rdi
	call	al_get_display_width
	movl	%eax, %edx
	shrl	$31, %edx
	addl	%edx, %eax
	sarl	%eax
	movl	%eax, %edx
	movq	-104(%rbp), %rax
	movl	$0, %r8d
	movl	%ebx, %ecx
	movl	$11, %esi
	movq	%rax, %rdi
	call	_ZN10block_list9new_blockEiiib
	nop
.L20:
	jmp	.L9
.L19:
	movq	-136(%rbp), %rax
	movl	$0, %esi
	pxor	%xmm1, %xmm1
	pxor	%xmm0, %xmm0
	movq	%rax, %rdi
	call	al_draw_bitmap
	movl	$255, %edx
	movl	$255, %esi
	movl	$255, %edi
	call	al_map_rgb
	movq	%xmm0, %rax
	movq	%xmm1, %rcx
	movq	%rcx, %rdx
	movq	%rax, %rcx
	movq	%rdx, %rbx
	movq	%rdx, %rax
	movq	%rcx, -184(%rbp)
	movq	-184(%rbp), %xmm4
	movq	%rax, -184(%rbp)
	movq	-184(%rbp), %xmm5
	movss	.LC42(%rip), %xmm3
	movss	.LC43(%rip), %xmm2
	movss	.LC44(%rip), %xmm1
	movss	.LC45(%rip), %xmm0
	call	al_draw_filled_rectangle
	movq	BLOCKTYPE+128(%rip), %rax
	movl	$0, %esi
	movss	.LC46(%rip), %xmm1
	movss	.LC47(%rip), %xmm0
	movq	%rax, %rdi
	call	al_draw_bitmap
	movq	CON_POINT(%rip), %rax
	movq	%rax, %rdi
	call	al_get_bitmap_height
	movl	%eax, %edx
	shrl	$31, %edx
	addl	%edx, %eax
	sarl	%eax
	movl	$380, %edx
	subl	%eax, %edx
	movl	%edx, %eax
	pxor	%xmm7, %xmm7
	cvtsi2ss	%eax, %xmm7
	movss	%xmm7, -184(%rbp)
	movq	CON_POINT(%rip), %rax
	movq	%rax, %rdi
	call	al_get_bitmap_width
	movl	%eax, %edx
	shrl	$31, %edx
	addl	%edx, %eax
	sarl	%eax
	movl	$70, %edx
	subl	%eax, %edx
	movl	%edx, %eax
	pxor	%xmm0, %xmm0
	cvtsi2ss	%eax, %xmm0
	movq	CON_POINT(%rip), %rax
	movl	$0, %esi
	movss	-184(%rbp), %xmm1
	movq	%rax, %rdi
	call	al_draw_bitmap
	movq	CON_POINT(%rip), %rax
	movq	%rax, %rdi
	call	al_get_bitmap_height
	movl	%eax, %edx
	shrl	$31, %edx
	addl	%edx, %eax
	sarl	%eax
	movl	$380, %edx
	subl	%eax, %edx
	movl	%edx, %eax
	pxor	%xmm7, %xmm7
	cvtsi2ss	%eax, %xmm7
	movss	%xmm7, -184(%rbp)
	movq	BLOCKTYPE+128(%rip), %rax
	movq	%rax, %rdi
	call	al_get_bitmap_width
	leal	70(%rax), %ebx
	movq	CON_POINT(%rip), %rax
	movq	%rax, %rdi
	call	al_get_bitmap_width
	movl	%eax, %edx
	shrl	$31, %edx
	addl	%edx, %eax
	sarl	%eax
	subl	%eax, %ebx
	movl	%ebx, %eax
	pxor	%xmm0, %xmm0
	cvtsi2ss	%eax, %xmm0
	movq	CON_POINT(%rip), %rax
	movl	$0, %esi
	movss	-184(%rbp), %xmm1
	movq	%rax, %rdi
	call	al_draw_bitmap
	movq	BLOCKTYPE+128(%rip), %rax
	movq	%rax, %rdi
	call	al_get_bitmap_height
	leal	380(%rax), %ebx
	movq	CON_POINT(%rip), %rax
	movq	%rax, %rdi
	call	al_get_bitmap_height
	movl	%eax, %edx
	shrl	$31, %edx
	addl	%edx, %eax
	sarl	%eax
	subl	%eax, %ebx
	movl	%ebx, %eax
	pxor	%xmm7, %xmm7
	cvtsi2ss	%eax, %xmm7
	movss	%xmm7, -184(%rbp)
	movq	BLOCKTYPE+128(%rip), %rax
	movq	%rax, %rdi
	call	al_get_bitmap_width
	movl	%eax, %edx
	shrl	$31, %edx
	addl	%edx, %eax
	sarl	%eax
	leal	70(%rax), %ebx
	movq	CON_POINT(%rip), %rax
	movq	%rax, %rdi
	call	al_get_bitmap_width
	movl	%eax, %edx
	shrl	$31, %edx
	addl	%edx, %eax
	sarl	%eax
	subl	%eax, %ebx
	movl	%ebx, %eax
	pxor	%xmm0, %xmm0
	cvtsi2ss	%eax, %xmm0
	movq	CON_POINT(%rip), %rax
	movl	$0, %esi
	movss	-184(%rbp), %xmm1
	movq	%rax, %rdi
	call	al_draw_bitmap
	movl	$0, %edx
	movl	$0, %esi
	movl	$0, %edi
	call	al_map_rgb
	movq	%xmm0, %rax
	movq	%xmm1, %rcx
	movq	%rcx, %rdx
	movq	%rax, %rcx
	movq	%rdx, %rbx
	movq	%rdx, %rax
	movss	.LC0(%rip), %xmm6
	movq	%rcx, -184(%rbp)
	movq	-184(%rbp), %xmm4
	movq	%rax, -184(%rbp)
	movq	-184(%rbp), %xmm5
	movss	.LC48(%rip), %xmm3
	movss	.LC49(%rip), %xmm2
	movss	.LC50(%rip), %xmm1
	movss	.LC51(%rip), %xmm0
	call	al_draw_rectangle
	movq	voltita(%rip), %rax
	movl	$0, %esi
	movss	.LC52(%rip), %xmm1
	movss	.LC53(%rip), %xmm0
	movq	%rax, %rdi
	call	al_draw_bitmap
	movq	voltita(%rip), %rax
	movl	$0, %esi
	movss	.LC52(%rip), %xmm1
	movss	.LC53(%rip), %xmm0
	movq	%rax, %rdi
	call	al_draw_bitmap
	movl	$0, %edx
	movl	$0, %esi
	movl	$0, %edi
	call	al_map_rgb
	movq	%xmm0, %rax
	movq	%xmm1, %rcx
	movq	%rcx, %rdx
	movq	%rax, %rcx
	movq	%rdx, %rbx
	movq	%rdx, %rax
	movss	.LC0(%rip), %xmm6
	movq	%rcx, -184(%rbp)
	movq	-184(%rbp), %xmm4
	movq	%rax, -184(%rbp)
	movq	-184(%rbp), %xmm5
	movss	.LC48(%rip), %xmm3
	movss	.LC54(%rip), %xmm2
	movss	.LC50(%rip), %xmm1
	movss	.LC55(%rip), %xmm0
	call	al_draw_rectangle
	movq	seta(%rip), %rax
	movl	$0, %esi
	movss	.LC52(%rip), %xmm1
	movss	.LC56(%rip), %xmm0
	movq	%rax, %rdi
	call	al_draw_bitmap
	movl	$0, %edx
	movl	$0, %esi
	movl	$0, %edi
	call	al_map_rgb
	movq	%xmm0, %rax
	movq	%xmm1, %rcx
	movq	%rcx, %rdx
	movq	%rax, %rcx
	movq	%rdx, %rbx
	movq	%rdx, %rax
	movss	.LC0(%rip), %xmm6
	movq	%rcx, -184(%rbp)
	movq	-184(%rbp), %xmm4
	movq	%rax, -184(%rbp)
	movq	-184(%rbp), %xmm5
	movss	.LC57(%rip), %xmm3
	movss	.LC49(%rip), %xmm2
	movss	.LC58(%rip), %xmm1
	movss	.LC51(%rip), %xmm0
	call	al_draw_rectangle
	movq	ultrassom(%rip), %rax
	movl	$0, %esi
	movss	.LC59(%rip), %xmm1
	movss	.LC53(%rip), %xmm0
	movq	%rax, %rdi
	call	al_draw_bitmap
	movl	$0, %edx
	movl	$0, %esi
	movl	$0, %edi
	call	al_map_rgb
	movq	%xmm0, %rax
	movq	%xmm1, %rcx
	movq	%rcx, %rdx
	movq	%rax, %rcx
	movq	%rdx, %rbx
	movq	%rdx, %rax
	movss	.LC0(%rip), %xmm6
	movq	%rcx, -184(%rbp)
	movq	-184(%rbp), %xmm4
	movq	%rax, -184(%rbp)
	movq	-184(%rbp), %xmm5
	movss	.LC57(%rip), %xmm3
	movss	.LC54(%rip), %xmm2
	movss	.LC58(%rip), %xmm1
	movss	.LC55(%rip), %xmm0
	call	al_draw_rectangle
	movq	fita(%rip), %rax
	movl	$0, %esi
	movss	.LC59(%rip), %xmm1
	movss	.LC56(%rip), %xmm0
	movq	%rax, %rdi
	call	al_draw_bitmap
	movl	$0, %edx
	movl	$0, %esi
	movl	$0, %edi
	call	al_map_rgb
	movq	%xmm0, %rax
	movq	%xmm1, %rcx
	movq	%rcx, %rdx
	movq	%rax, %rcx
	movq	%rdx, %rbx
	movq	%rdx, %rax
	movss	.LC0(%rip), %xmm6
	movq	%rcx, -184(%rbp)
	movq	-184(%rbp), %xmm4
	movq	%rax, -184(%rbp)
	movq	-184(%rbp), %xmm5
	movss	.LC60(%rip), %xmm3
	movss	.LC49(%rip), %xmm2
	movss	.LC61(%rip), %xmm1
	movss	.LC51(%rip), %xmm0
	call	al_draw_rectangle
	movq	logic_true(%rip), %rax
	movl	$0, %esi
	movss	.LC62(%rip), %xmm1
	movss	.LC53(%rip), %xmm0
	movq	%rax, %rdi
	call	al_draw_bitmap
	movl	$0, %edx
	movl	$0, %esi
	movl	$0, %edi
	call	al_map_rgb
	movq	%xmm0, %rax
	movq	%xmm1, %rcx
	movq	%rcx, %rdx
	movq	%rax, %rcx
	movq	%rdx, %rbx
	movq	%rdx, %rax
	movss	.LC0(%rip), %xmm6
	movq	%rcx, -184(%rbp)
	movq	-184(%rbp), %xmm4
	movq	%rax, -184(%rbp)
	movq	-184(%rbp), %xmm5
	movss	.LC60(%rip), %xmm3
	movss	.LC54(%rip), %xmm2
	movss	.LC61(%rip), %xmm1
	movss	.LC55(%rip), %xmm0
	call	al_draw_rectangle
	movq	NUMBER(%rip), %rax
	movl	$0, %esi
	movss	.LC62(%rip), %xmm1
	movss	.LC56(%rip), %xmm0
	movq	%rax, %rdi
	call	al_draw_bitmap
	movl	mode(%rip), %eax
	cmpl	$1, %eax
	jne	.L35
	movl	$0, %edx
	movl	$0, %esi
	movl	$255, %edi
	call	al_map_rgb
	movq	%xmm0, %rax
	movq	%xmm1, %rcx
	movq	%rcx, %rdx
	movq	%rax, %rcx
	movq	%rdx, %rbx
	movq	%rdx, %rax
	movss	.LC0(%rip), %xmm6
	movq	%rcx, -184(%rbp)
	movq	-184(%rbp), %xmm4
	movq	%rax, -184(%rbp)
	movq	-184(%rbp), %xmm5
	movss	.LC63(%rip), %xmm3
	movss	.LC45(%rip), %xmm2
	movss	.LC64(%rip), %xmm1
	movss	.LC65(%rip), %xmm0
	call	al_draw_rectangle
	jmp	.L36
.L35:
	movl	mouse_x(%rip), %eax
	cmpl	$17, %eax
	jle	.L36
	movl	mouse_y(%rip), %eax
	cmpl	$9, %eax
	jle	.L36
	movl	mouse_x(%rip), %eax
	cmpl	$49, %eax
	jg	.L36
	movl	mouse_y(%rip), %eax
	cmpl	$41, %eax
	jg	.L36
	movl	$255, %edx
	movl	$255, %esi
	movl	$255, %edi
	call	al_map_rgb
	movq	%xmm0, %rax
	movq	%xmm1, %rcx
	movq	%rcx, %rdx
	movq	%rax, %rcx
	movq	%rdx, %rbx
	movq	%rdx, %rax
	movss	.LC0(%rip), %xmm6
	movq	%rcx, -184(%rbp)
	movq	-184(%rbp), %xmm4
	movq	%rax, -184(%rbp)
	movq	-184(%rbp), %xmm5
	movss	.LC63(%rip), %xmm3
	movss	.LC45(%rip), %xmm2
	movss	.LC64(%rip), %xmm1
	movss	.LC65(%rip), %xmm0
	call	al_draw_rectangle
	movzbl	press_mouse(%rip), %eax
	testb	%al, %al
	je	.L36
	movl	$1, mode(%rip)
	movq	-104(%rbp), %rax
	movzbl	16(%rax), %eax
	testb	%al, %al
	je	.L36
	movq	-104(%rbp), %rax
	movb	$0, 16(%rax)
	movq	-104(%rbp), %rax
	movq	8(%rax), %rax
	movb	$0, (%rax)
	movq	-104(%rbp), %rax
	movq	$0, 8(%rax)
.L36:
	movl	mode(%rip), %eax
	testl	%eax, %eax
	jne	.L37
	movl	$0, %edx
	movl	$0, %esi
	movl	$255, %edi
	call	al_map_rgb
	movq	%xmm0, %rax
	movq	%xmm1, %rcx
	movq	%rcx, %rdx
	movq	%rax, %rcx
	movq	%rdx, %rbx
	movq	%rdx, %rax
	movss	.LC0(%rip), %xmm6
	movq	%rcx, -184(%rbp)
	movq	-184(%rbp), %xmm4
	movq	%rax, -184(%rbp)
	movq	-184(%rbp), %xmm5
	movss	.LC63(%rip), %xmm3
	movss	.LC66(%rip), %xmm2
	movss	.LC64(%rip), %xmm1
	movss	.LC67(%rip), %xmm0
	call	al_draw_rectangle
	jmp	.L38
.L37:
	movl	mouse_x(%rip), %eax
	cmpl	$51, %eax
	jle	.L38
	movl	mouse_y(%rip), %eax
	cmpl	$9, %eax
	jle	.L38
	movl	mouse_x(%rip), %eax
	cmpl	$82, %eax
	jg	.L38
	movl	mouse_y(%rip), %eax
	cmpl	$41, %eax
	jg	.L38
	movl	$255, %edx
	movl	$255, %esi
	movl	$255, %edi
	call	al_map_rgb
	movq	%xmm0, %rax
	movq	%xmm1, %rcx
	movq	%rcx, %rdx
	movq	%rax, %rcx
	movq	%rdx, %rbx
	movq	%rdx, %rax
	movss	.LC0(%rip), %xmm6
	movq	%rcx, -184(%rbp)
	movq	-184(%rbp), %xmm4
	movq	%rax, -184(%rbp)
	movq	-184(%rbp), %xmm5
	movss	.LC63(%rip), %xmm3
	movss	.LC66(%rip), %xmm2
	movss	.LC64(%rip), %xmm1
	movss	.LC67(%rip), %xmm0
	call	al_draw_rectangle
	movzbl	press_mouse(%rip), %eax
	testb	%al, %al
	je	.L38
	movl	mode(%rip), %eax
	cmpl	$1, %eax
	jne	.L38
	movl	$0, mode(%rip)
.L38:
	movl	mode(%rip), %eax
	cmpl	$-1, %eax
	jne	.L39
	movl	$0, %edx
	movl	$0, %esi
	movl	$255, %edi
	call	al_map_rgb
	movq	%xmm0, %rax
	movq	%xmm1, %rcx
	movq	%rcx, %rdx
	movq	%rax, %rcx
	movq	%rdx, %rbx
	movq	%rdx, %rax
	movss	.LC0(%rip), %xmm6
	movq	%rcx, -184(%rbp)
	movq	-184(%rbp), %xmm4
	movq	%rax, -184(%rbp)
	movq	-184(%rbp), %xmm5
	movss	.LC63(%rip), %xmm3
	movss	.LC68(%rip), %xmm2
	movss	.LC64(%rip), %xmm1
	movss	.LC69(%rip), %xmm0
	call	al_draw_rectangle
	jmp	.L40
.L39:
	movl	mouse_x(%rip), %eax
	cmpl	$82, %eax
	jle	.L40
	movl	mouse_y(%rip), %eax
	cmpl	$9, %eax
	jle	.L40
	movl	mouse_x(%rip), %eax
	cmpl	$115, %eax
	jg	.L40
	movl	mouse_y(%rip), %eax
	cmpl	$41, %eax
	jg	.L40
	movl	$255, %edx
	movl	$255, %esi
	movl	$255, %edi
	call	al_map_rgb
	movq	%xmm0, %rax
	movq	%xmm1, %rcx
	movq	%rcx, %rdx
	movq	%rax, %rcx
	movq	%rdx, %rbx
	movq	%rdx, %rax
	movss	.LC0(%rip), %xmm6
	movq	%rcx, -184(%rbp)
	movq	-184(%rbp), %xmm4
	movq	%rax, -184(%rbp)
	movq	-184(%rbp), %xmm5
	movss	.LC63(%rip), %xmm3
	movss	.LC68(%rip), %xmm2
	movss	.LC64(%rip), %xmm1
	movss	.LC69(%rip), %xmm0
	call	al_draw_rectangle
	movzbl	press_mouse(%rip), %eax
	testb	%al, %al
	je	.L40
	movl	$-1, mode(%rip)
	movq	-104(%rbp), %rax
	movzbl	16(%rax), %eax
	testb	%al, %al
	je	.L40
	movq	-104(%rbp), %rax
	movb	$0, 16(%rax)
	movq	-104(%rbp), %rax
	movq	8(%rax), %rax
	movb	$0, (%rax)
	movq	-104(%rbp), %rax
	movq	$0, 8(%rax)
.L40:
	movl	mouse_x(%rip), %eax
	cmpl	$117, %eax
	jle	.L41
	movl	mouse_y(%rip), %eax
	cmpl	$9, %eax
	jle	.L41
	movl	mouse_x(%rip), %eax
	cmpl	$149, %eax
	jg	.L41
	movl	mouse_y(%rip), %eax
	cmpl	$41, %eax
	jg	.L41
	movl	$255, %edx
	movl	$255, %esi
	movl	$255, %edi
	call	al_map_rgb
	movq	%xmm0, %rax
	movq	%xmm1, %rcx
	movq	%rcx, %rdx
	movq	%rax, %rcx
	movq	%rdx, %rbx
	movq	%rdx, %rax
	movss	.LC0(%rip), %xmm6
	movq	%rcx, -184(%rbp)
	movq	-184(%rbp), %xmm4
	movq	%rax, -184(%rbp)
	movq	-184(%rbp), %xmm5
	movss	.LC63(%rip), %xmm3
	movss	.LC70(%rip), %xmm2
	movss	.LC64(%rip), %xmm1
	movss	.LC71(%rip), %xmm0
	call	al_draw_rectangle
.L41:
	movl	mode(%rip), %eax
	cmpl	$-1, %eax
	jne	.L42
	movq	-104(%rbp), %rax
	movq	%rax, %rdi
	call	_ZN10block_list6updateEv
	movl	mouse_x(%rip), %eax
	cmpl	$50, %eax
	jle	.L43
	movl	mouse_y(%rip), %eax
	cmpl	$65, %eax
	jle	.L43
	movl	mouse_x(%rip), %eax
	cmpl	$143, %eax
	jg	.L43
	movl	mouse_y(%rip), %eax
	cmpl	$106, %eax
	jg	.L43
	movl	$112, %edx
	movl	$160, %esi
	movl	$32, %edi
	call	al_map_rgb
	movq	%xmm0, %rax
	movq	%xmm1, %rcx
	movq	%rcx, %rdx
	movq	%rax, %rcx
	movq	%rdx, %rbx
	movq	%rdx, %rax
	movss	.LC0(%rip), %xmm6
	movq	%rcx, -184(%rbp)
	movq	-184(%rbp), %xmm4
	movq	%rax, -184(%rbp)
	movq	-184(%rbp), %xmm5
	movss	.LC72(%rip), %xmm3
	movss	.LC73(%rip), %xmm2
	movss	.LC74(%rip), %xmm1
	movss	.LC45(%rip), %xmm0
	call	al_draw_rectangle
	movzbl	press_mouse(%rip), %eax
	testb	%al, %al
	je	.L43
	movl	start_block(%rip), %eax
	testl	%eax, %eax
	jne	.L43
	movl	mouse_y(%rip), %ecx
	movl	mouse_x(%rip), %edx
	movq	-104(%rbp), %rax
	movl	$1, %r8d
	movl	$3, %esi
	movq	%rax, %rdi
	call	_ZN10block_list9new_blockEiiib
	movl	start_block(%rip), %eax
	addl	$1, %eax
	movl	%eax, start_block(%rip)
.L43:
	movl	mouse_x(%rip), %eax
	cmpl	$49, %eax
	jle	.L44
	movl	mouse_y(%rip), %eax
	cmpl	$127, %eax
	jle	.L44
	movl	mouse_x(%rip), %eax
	cmpl	$143, %eax
	jg	.L44
	movl	mouse_y(%rip), %eax
	cmpl	$178, %eax
	jg	.L44
	movl	$112, %edx
	movl	$160, %esi
	movl	$32, %edi
	call	al_map_rgb
	movq	%xmm0, %rax
	movq	%xmm1, %rcx
	movq	%rcx, %rdx
	movq	%rax, %rcx
	movq	%rdx, %rbx
	movq	%rdx, %rax
	movss	.LC0(%rip), %xmm6
	movq	%rcx, -184(%rbp)
	movq	-184(%rbp), %xmm4
	movq	%rax, -184(%rbp)
	movq	-184(%rbp), %xmm5
	movss	.LC75(%rip), %xmm3
	movss	.LC73(%rip), %xmm2
	movss	.LC76(%rip), %xmm1
	movss	.LC77(%rip), %xmm0
	call	al_draw_rectangle
	movzbl	press_mouse(%rip), %eax
	testb	%al, %al
	je	.L44
	movl	mouse_y(%rip), %ecx
	movl	mouse_x(%rip), %edx
	movq	-104(%rbp), %rax
	movl	$1, %r8d
	movl	$4, %esi
	movq	%rax, %rdi
	call	_ZN10block_list9new_blockEiiib
.L44:
	movl	mouse_x(%rip), %eax
	cmpl	$47, %eax
	jle	.L45
	movl	mouse_y(%rip), %eax
	cmpl	$193, %eax
	jle	.L45
	movl	mouse_x(%rip), %eax
	cmpl	$147, %eax
	jg	.L45
	movl	mouse_y(%rip), %eax
	cmpl	$249, %eax
	jg	.L45
	movl	$112, %edx
	movl	$160, %esi
	movl	$32, %edi
	call	al_map_rgb
	movq	%xmm0, %rax
	movq	%xmm1, %rcx
	movq	%rcx, %rdx
	movq	%rax, %rcx
	movq	%rdx, %rbx
	movq	%rdx, %rax
	movss	.LC0(%rip), %xmm6
	movq	%rcx, -184(%rbp)
	movq	-184(%rbp), %xmm4
	movq	%rax, -184(%rbp)
	movq	-184(%rbp), %xmm5
	movss	.LC78(%rip), %xmm3
	movss	.LC79(%rip), %xmm2
	movss	.LC80(%rip), %xmm1
	movss	.LC53(%rip), %xmm0
	call	al_draw_rectangle
	movzbl	press_mouse(%rip), %eax
	testb	%al, %al
	je	.L45
	movl	mouse_y(%rip), %ecx
	movl	mouse_x(%rip), %edx
	movq	-104(%rbp), %rax
	movl	$1, %r8d
	movl	$2, %esi
	movq	%rax, %rdi
	call	_ZN10block_list9new_blockEiiib
.L45:
	movl	mouse_x(%rip), %eax
	cmpl	$47, %eax
	jle	.L46
	movl	mouse_y(%rip), %eax
	cmpl	$283, %eax
	jle	.L46
	movl	mouse_x(%rip), %eax
	cmpl	$147, %eax
	jg	.L46
	movl	mouse_y(%rip), %eax
	cmpl	$320, %eax
	jg	.L46
	movl	$112, %edx
	movl	$160, %esi
	movl	$32, %edi
	call	al_map_rgb
	movq	%xmm0, %rax
	movq	%xmm1, %rcx
	movq	%rcx, %rdx
	movq	%rax, %rcx
	movq	%rdx, %rbx
	movq	%rdx, %rax
	movss	.LC0(%rip), %xmm6
	movq	%rcx, -184(%rbp)
	movq	-184(%rbp), %xmm4
	movq	%rax, -184(%rbp)
	movq	-184(%rbp), %xmm5
	movss	.LC81(%rip), %xmm3
	movss	.LC79(%rip), %xmm2
	movss	.LC82(%rip), %xmm1
	movss	.LC83(%rip), %xmm0
	call	al_draw_rectangle
	movzbl	press_mouse(%rip), %eax
	testb	%al, %al
	je	.L46
	movl	mouse_y(%rip), %ecx
	movl	mouse_x(%rip), %edx
	movq	-104(%rbp), %rax
	movl	$1, %r8d
	movl	$1, %esi
	movq	%rax, %rdi
	call	_ZN10block_list9new_blockEiiib
.L46:
	movq	voltita(%rip), %rax
	movq	%rax, %rdx
	movl	$340, %esi
	movl	$47, %edi
	call	_Z10mouse_overiiP14ALLEGRO_BITMAP
	testb	%al, %al
	je	.L47
	movl	$1, -144(%rbp)
	movzbl	press_mouse(%rip), %eax
	testb	%al, %al
	je	.L47
	movl	mouse_y(%rip), %ecx
	movl	mouse_x(%rip), %edx
	movq	-104(%rbp), %rax
	movl	$1, %r8d
	movl	$7, %esi
	movq	%rax, %rdi
	call	_ZN10block_list9new_blockEiiib
.L47:
	movl	-144(%rbp), %eax
	negl	%eax
	movzbl	%al, %edx
	movl	-144(%rbp), %eax
	negl	%eax
	movzbl	%al, %ecx
	movl	-144(%rbp), %eax
	negl	%eax
	movzbl	%al, %eax
	movl	%ecx, %esi
	movl	%eax, %edi
	call	al_map_rgb
	movq	%xmm0, %rax
	movq	%xmm1, %rcx
	movq	%rcx, %rdx
	movq	%rax, %rcx
	movq	%rdx, %rbx
	movq	%rdx, %rax
	movss	.LC0(%rip), %xmm6
	movq	%rcx, -184(%rbp)
	movq	-184(%rbp), %xmm4
	movq	%rax, -184(%rbp)
	movq	-184(%rbp), %xmm5
	movss	.LC48(%rip), %xmm3
	movss	.LC49(%rip), %xmm2
	movss	.LC50(%rip), %xmm1
	movss	.LC51(%rip), %xmm0
	call	al_draw_rectangle
	movl	$0, -144(%rbp)
	movq	seta(%rip), %rax
	movq	%rax, %rdx
	movl	$340, %esi
	movl	$80, %edi
	call	_Z10mouse_overiiP14ALLEGRO_BITMAP
	testb	%al, %al
	je	.L48
	movl	$1, -144(%rbp)
	movzbl	press_mouse(%rip), %eax
	testb	%al, %al
	je	.L48
	movl	mouse_y(%rip), %ecx
	movl	mouse_x(%rip), %edx
	movq	-104(%rbp), %rax
	movl	$1, %r8d
	movl	$6, %esi
	movq	%rax, %rdi
	call	_ZN10block_list9new_blockEiiib
.L48:
	movl	-144(%rbp), %eax
	negl	%eax
	movzbl	%al, %edx
	movl	-144(%rbp), %eax
	negl	%eax
	movzbl	%al, %ecx
	movl	-144(%rbp), %eax
	negl	%eax
	movzbl	%al, %eax
	movl	%ecx, %esi
	movl	%eax, %edi
	call	al_map_rgb
	movq	%xmm0, %rax
	movq	%xmm1, %rcx
	movq	%rcx, %rdx
	movq	%rax, %rcx
	movq	%rdx, %rbx
	movq	%rdx, %rax
	movss	.LC0(%rip), %xmm6
	movq	%rcx, -184(%rbp)
	movq	-184(%rbp), %xmm4
	movq	%rax, -184(%rbp)
	movq	-184(%rbp), %xmm5
	movss	.LC48(%rip), %xmm3
	movss	.LC54(%rip), %xmm2
	movss	.LC50(%rip), %xmm1
	movss	.LC55(%rip), %xmm0
	call	al_draw_rectangle
	movl	$0, -144(%rbp)
	movq	ultrassom(%rip), %rax
	movq	%rax, %rdx
	movl	$501, %esi
	movl	$47, %edi
	call	_Z10mouse_overiiP14ALLEGRO_BITMAP
	testb	%al, %al
	je	.L49
	movl	$1, -144(%rbp)
	movzbl	press_mouse(%rip), %eax
	testb	%al, %al
	je	.L49
	movl	mouse_y(%rip), %ecx
	movl	mouse_x(%rip), %edx
	movq	-104(%rbp), %rax
	movl	$1, %r8d
	movl	$9, %esi
	movq	%rax, %rdi
	call	_ZN10block_list9new_blockEiiib
.L49:
	movl	-144(%rbp), %eax
	negl	%eax
	movzbl	%al, %edx
	movl	-144(%rbp), %eax
	negl	%eax
	movzbl	%al, %ecx
	movl	-144(%rbp), %eax
	negl	%eax
	movzbl	%al, %eax
	movl	%ecx, %esi
	movl	%eax, %edi
	call	al_map_rgb
	movq	%xmm0, %rax
	movq	%xmm1, %rcx
	movq	%rcx, %rdx
	movq	%rax, %rcx
	movq	%rdx, %rbx
	movq	%rdx, %rax
	movss	.LC0(%rip), %xmm6
	movq	%rcx, -184(%rbp)
	movq	-184(%rbp), %xmm4
	movq	%rax, -184(%rbp)
	movq	-184(%rbp), %xmm5
	movss	.LC57(%rip), %xmm3
	movss	.LC49(%rip), %xmm2
	movss	.LC58(%rip), %xmm1
	movss	.LC51(%rip), %xmm0
	call	al_draw_rectangle
	movl	$0, -144(%rbp)
	movq	fita(%rip), %rax
	movq	%rax, %rdx
	movl	$501, %esi
	movl	$80, %edi
	call	_Z10mouse_overiiP14ALLEGRO_BITMAP
	testb	%al, %al
	je	.L50
	movl	$1, -144(%rbp)
	movzbl	press_mouse(%rip), %eax
	testb	%al, %al
	je	.L50
	movl	mouse_y(%rip), %ecx
	movl	mouse_x(%rip), %edx
	movq	-104(%rbp), %rax
	movl	$1, %r8d
	movl	$8, %esi
	movq	%rax, %rdi
	call	_ZN10block_list9new_blockEiiib
.L50:
	movl	-144(%rbp), %eax
	negl	%eax
	movzbl	%al, %edx
	movl	-144(%rbp), %eax
	negl	%eax
	movzbl	%al, %ecx
	movl	-144(%rbp), %eax
	negl	%eax
	movzbl	%al, %eax
	movl	%ecx, %esi
	movl	%eax, %edi
	call	al_map_rgb
	movq	%xmm0, %rax
	movq	%xmm1, %rcx
	movq	%rcx, %rdx
	movq	%rax, %rcx
	movq	%rdx, %rbx
	movq	%rdx, %rax
	movss	.LC0(%rip), %xmm6
	movq	%rcx, -184(%rbp)
	movq	-184(%rbp), %xmm4
	movq	%rax, -184(%rbp)
	movq	-184(%rbp), %xmm5
	movss	.LC57(%rip), %xmm3
	movss	.LC54(%rip), %xmm2
	movss	.LC58(%rip), %xmm1
	movss	.LC55(%rip), %xmm0
	call	al_draw_rectangle
	movl	$0, -144(%rbp)
	movq	logic_true(%rip), %rax
	movq	%rax, %rdx
	movl	$531, %esi
	movl	$47, %edi
	call	_Z10mouse_overiiP14ALLEGRO_BITMAP
	testb	%al, %al
	je	.L51
	movl	$1, -144(%rbp)
	movzbl	press_mouse(%rip), %eax
	testb	%al, %al
	je	.L51
	movl	mouse_y(%rip), %ecx
	movl	mouse_x(%rip), %edx
	movq	-104(%rbp), %rax
	movl	$1, %r8d
	movl	$11, %esi
	movq	%rax, %rdi
	call	_ZN10block_list9new_blockEiiib
.L51:
	movl	-144(%rbp), %eax
	negl	%eax
	movzbl	%al, %edx
	movl	-144(%rbp), %eax
	negl	%eax
	movzbl	%al, %ecx
	movl	-144(%rbp), %eax
	negl	%eax
	movzbl	%al, %eax
	movl	%ecx, %esi
	movl	%eax, %edi
	call	al_map_rgb
	movq	%xmm0, %rax
	movq	%xmm1, %rcx
	movq	%rcx, %rdx
	movq	%rax, %rcx
	movq	%rdx, %rbx
	movq	%rdx, %rax
	movss	.LC0(%rip), %xmm6
	movq	%rcx, -184(%rbp)
	movq	-184(%rbp), %xmm4
	movq	%rax, -184(%rbp)
	movq	-184(%rbp), %xmm5
	movss	.LC60(%rip), %xmm3
	movss	.LC49(%rip), %xmm2
	movss	.LC61(%rip), %xmm1
	movss	.LC51(%rip), %xmm0
	call	al_draw_rectangle
	movl	$0, -144(%rbp)
	movq	NUMBER(%rip), %rax
	movq	%rax, %rdx
	movl	$531, %esi
	movl	$80, %edi
	call	_Z10mouse_overiiP14ALLEGRO_BITMAP
	testb	%al, %al
	je	.L52
	movl	$1, -144(%rbp)
	movzbl	press_mouse(%rip), %eax
	testb	%al, %al
	je	.L52
	movl	mouse_y(%rip), %ecx
	movl	mouse_x(%rip), %edx
	movq	-104(%rbp), %rax
	movl	$1, %r8d
	movl	$10, %esi
	movq	%rax, %rdi
	call	_ZN10block_list9new_blockEiiib
.L52:
	movl	-144(%rbp), %eax
	negl	%eax
	movzbl	%al, %edx
	movl	-144(%rbp), %eax
	negl	%eax
	movzbl	%al, %ecx
	movl	-144(%rbp), %eax
	negl	%eax
	movzbl	%al, %eax
	movl	%ecx, %esi
	movl	%eax, %edi
	call	al_map_rgb
	movq	%xmm0, %rax
	movq	%xmm1, %rcx
	movq	%rcx, %rdx
	movq	%rax, %rcx
	movq	%rdx, %rbx
	movq	%rdx, %rax
	movss	.LC0(%rip), %xmm6
	movq	%rcx, -184(%rbp)
	movq	-184(%rbp), %xmm4
	movq	%rax, -184(%rbp)
	movq	-184(%rbp), %xmm5
	movss	.LC60(%rip), %xmm3
	movss	.LC54(%rip), %xmm2
	movss	.LC61(%rip), %xmm1
	movss	.LC55(%rip), %xmm0
	call	al_draw_rectangle
	movl	$0, -144(%rbp)
	movl	mouse_x(%rip), %eax
	cmpl	$50, %eax
	jle	.L53
	movl	mouse_y(%rip), %eax
	cmpl	$370, %eax
	jle	.L53
	movl	mouse_x(%rip), %eax
	cmpl	$144, %eax
	jg	.L53
	movl	mouse_y(%rip), %eax
	cmpl	$444, %eax
	jg	.L53
	movl	$112, %edx
	movl	$160, %esi
	movl	$32, %edi
	call	al_map_rgb
	movq	%xmm0, %rax
	movq	%xmm1, %rcx
	movq	%rcx, %rdx
	movq	%rax, %rcx
	movq	%rdx, %rbx
	movq	%rdx, %rax
	movss	.LC0(%rip), %xmm6
	movq	%rcx, -184(%rbp)
	movq	-184(%rbp), %xmm4
	movq	%rax, -184(%rbp)
	movq	-184(%rbp), %xmm5
	movss	.LC42(%rip), %xmm3
	movss	.LC43(%rip), %xmm2
	movss	.LC44(%rip), %xmm1
	movss	.LC45(%rip), %xmm0
	call	al_draw_rectangle
	movzbl	press_mouse(%rip), %eax
	testb	%al, %al
	je	.L53
	movl	mouse_y(%rip), %ecx
	movl	mouse_x(%rip), %edx
	movq	-104(%rbp), %rax
	movl	$1, %r8d
	movl	$5, %esi
	movq	%rax, %rdi
	call	_ZN10block_list9new_blockEiiib
	jmp	.L53
.L42:
	movl	start_block(%rip), %eax
	cmpl	$1, %eax
	jne	.L53
	movq	-104(%rbp), %rax
	movq	%rax, %rdi
	call	_ZN10block_list4playEv
	movq	-104(%rbp), %rax
	movzbl	16(%rax), %eax
	xorl	$1, %eax
	testb	%al, %al
	je	.L53
	movl	$-1, mode(%rip)
.L53:
	movq	-104(%rbp), %rax
	movq	%rax, %rdi
	call	_ZN10block_list4drawEv
	movl	$0, %edx
	movl	$0, %esi
	movl	$0, %edi
	call	al_map_rgb
	movq	%xmm0, %rax
	movq	%xmm1, %rcx
	movq	%rcx, %rdx
	movq	%rax, %rcx
	movq	%rdx, %rbx
	movq	%rdx, %rax
	movq	%rcx, -184(%rbp)
	movq	-184(%rbp), %xmm4
	movq	%rax, -184(%rbp)
	movq	-184(%rbp), %xmm5
	movss	.LC84(%rip), %xmm3
	movss	.LC85(%rip), %xmm2
	movss	.LC86(%rip), %xmm1
	movss	.LC87(%rip), %xmm0
	call	al_draw_filled_rectangle
	movl	mouse_x(%rip), %eax
	pxor	%xmm0, %xmm0
	cvtsi2ss	%eax, %xmm0
	movss	scale(%rip), %xmm2
	movss	.LC88(%rip), %xmm1
	mulss	%xmm2, %xmm1
	movss	.LC89(%rip), %xmm2
	addss	%xmm2, %xmm1
	ucomiss	%xmm1, %xmm0
	jbe	.L54
	movl	mouse_y(%rip), %eax
	cmpl	$566, %eax
	jle	.L54
	movl	mouse_x(%rip), %eax
	pxor	%xmm1, %xmm1
	cvtsi2ss	%eax, %xmm1
	movss	scale(%rip), %xmm2
	movss	.LC88(%rip), %xmm0
	mulss	%xmm2, %xmm0
	movss	.LC90(%rip), %xmm2
	addss	%xmm2, %xmm0
	ucomiss	%xmm1, %xmm0
	jbe	.L54
	movl	mouse_y(%rip), %eax
	cmpl	$578, %eax
	jg	.L54
	movl	$1, -144(%rbp)
	movzbl	press_mouse(%rip), %eax
	testb	%al, %al
	je	.L58
	movb	$1, -146(%rbp)
	movl	mouse_x(%rip), %eax
	pxor	%xmm0, %xmm0
	cvtsi2ss	%eax, %xmm0
	movss	scale(%rip), %xmm2
	movss	.LC88(%rip), %xmm1
	mulss	%xmm2, %xmm1
	movss	.LC91(%rip), %xmm2
	addss	%xmm2, %xmm1
	subss	%xmm1, %xmm0
	cvttss2si	%xmm0, %eax
	movl	%eax, dmouse_x(%rip)
	jmp	.L58
.L54:
	movl	$0, -144(%rbp)
.L58:
	movzbl	press_mouse(%rip), %eax
	testb	%al, %al
	je	.L59
	movl	mouse_x(%rip), %eax
	cmpl	$700, %eax
	jle	.L59
	movl	mouse_y(%rip), %eax
	cmpl	$570, %eax
	jle	.L59
	movl	mouse_x(%rip), %eax
	cmpl	$779, %eax
	jg	.L59
	movl	mouse_y(%rip), %eax
	cmpl	$574, %eax
	jg	.L59
	movl	mouse_x(%rip), %eax
	subl	$691, %eax
	pxor	%xmm0, %xmm0
	cvtsi2sd	%eax, %xmm0
	movsd	.LC92(%rip), %xmm1
	divsd	%xmm1, %xmm0
	cvtsd2ss	%xmm0, %xmm0
	movss	%xmm0, scale(%rip)
	movss	scale(%rip), %xmm0
	cvtss2sd	%xmm0, %xmm0
	movsd	.LC93(%rip), %xmm1
	ucomisd	%xmm0, %xmm1
	jbe	.L80
	movss	.LC94(%rip), %xmm0
	movss	%xmm0, scale(%rip)
	jmp	.L59
.L80:
	movss	scale(%rip), %xmm0
	ucomiss	.LC95(%rip), %xmm0
	jbe	.L59
	movss	.LC95(%rip), %xmm0
	movss	%xmm0, scale(%rip)
.L59:
	movzbl	hold_mouse(%rip), %eax
	testb	%al, %al
	je	.L63
	cmpb	$0, -146(%rbp)
	je	.L63
	movl	mouse_x(%rip), %edx
	movl	dmouse_x(%rip), %eax
	subl	%eax, %edx
	movl	%edx, %eax
	subl	$691, %eax
	pxor	%xmm0, %xmm0
	cvtsi2sd	%eax, %xmm0
	movsd	.LC92(%rip), %xmm1
	divsd	%xmm1, %xmm0
	cvtsd2ss	%xmm0, %xmm0
	movss	%xmm0, scale(%rip)
	movss	scale(%rip), %xmm0
	cvtss2sd	%xmm0, %xmm0
	movsd	.LC93(%rip), %xmm1
	ucomisd	%xmm0, %xmm1
	jbe	.L81
	movss	.LC94(%rip), %xmm0
	movss	%xmm0, scale(%rip)
	jmp	.L63
.L81:
	movss	scale(%rip), %xmm0
	ucomiss	.LC95(%rip), %xmm0
	jbe	.L63
	movss	.LC95(%rip), %xmm0
	movss	%xmm0, scale(%rip)
.L63:
	cmpb	$0, -146(%rbp)
	je	.L67
	movzbl	release_mouse(%rip), %eax
	testb	%al, %al
	je	.L67
	movb	$0, -146(%rbp)
	movl	$0, dmouse_x(%rip)
.L67:
	movl	-144(%rbp), %eax
	negl	%eax
	movzbl	%al, %edx
	movl	-144(%rbp), %eax
	negl	%eax
	movzbl	%al, %ecx
	movl	-144(%rbp), %eax
	negl	%eax
	movzbl	%al, %eax
	movl	%ecx, %esi
	movl	%eax, %edi
	call	al_map_rgb
	movq	%xmm0, %rax
	movq	%xmm1, %rcx
	movq	%rcx, %rdx
	movss	scale(%rip), %xmm1
	movss	.LC88(%rip), %xmm0
	mulss	%xmm1, %xmm0
	movss	.LC96(%rip), %xmm1
	movaps	%xmm0, %xmm2
	addss	%xmm1, %xmm2
	movss	scale(%rip), %xmm1
	movss	.LC88(%rip), %xmm0
	mulss	%xmm1, %xmm0
	movss	.LC89(%rip), %xmm1
	addss	%xmm1, %xmm0
	movq	%rax, %rcx
	movq	%rdx, %rbx
	movq	%rdx, %rax
	movss	.LC0(%rip), %xmm6
	movq	%rcx, -184(%rbp)
	movq	-184(%rbp), %xmm4
	movq	%rax, -184(%rbp)
	movq	-184(%rbp), %xmm5
	movss	.LC97(%rip), %xmm3
	movss	.LC98(%rip), %xmm1
	call	al_draw_rectangle
	movl	$128, %edx
	movl	$128, %esi
	movl	$128, %edi
	call	al_map_rgb
	movq	%xmm0, %rax
	movq	%xmm1, %rcx
	movq	%rcx, %rdx
	movss	scale(%rip), %xmm1
	movss	.LC88(%rip), %xmm0
	mulss	%xmm1, %xmm0
	movss	.LC90(%rip), %xmm1
	movaps	%xmm0, %xmm2
	addss	%xmm1, %xmm2
	movss	scale(%rip), %xmm1
	movss	.LC88(%rip), %xmm0
	mulss	%xmm1, %xmm0
	movss	.LC99(%rip), %xmm1
	addss	%xmm1, %xmm0
	movq	%rax, %rcx
	movq	%rdx, %rbx
	movq	%rdx, %rax
	movq	%rcx, -184(%rbp)
	movq	-184(%rbp), %xmm4
	movq	%rax, -184(%rbp)
	movq	-184(%rbp), %xmm5
	movss	.LC100(%rip), %xmm3
	movss	.LC101(%rip), %xmm1
	call	al_draw_filled_rectangle
	call	al_flip_display
	movl	$127, %edx
	movl	$127, %esi
	movl	$127, %edi
	call	al_map_rgb
	movq	%xmm0, %rax
	movq	%xmm1, %rcx
	movq	%rcx, %rdx
	movq	%rax, %rcx
	movq	%rdx, %rbx
	movq	%rdx, %rax
	movq	%rcx, -184(%rbp)
	movq	-184(%rbp), %xmm0
	movq	%rax, -184(%rbp)
	movq	-184(%rbp), %xmm1
	call	al_clear_to_color
	jmp	.L68
.L2:
	movq	seta(%rip), %rax
	movq	%rax, %rdi
	call	al_destroy_bitmap
	movq	voltita(%rip), %rax
	movq	%rax, %rdi
	call	al_destroy_bitmap
	movq	ultrassom(%rip), %rax
	movq	%rax, %rdi
	call	al_destroy_bitmap
	movq	logic_true(%rip), %rax
	movq	%rax, %rdi
	call	al_destroy_bitmap
	movq	logic_false(%rip), %rax
	movq	%rax, %rdi
	call	al_destroy_bitmap
	movq	fita(%rip), %rax
	movq	%rax, %rdi
	call	al_destroy_bitmap
	movq	CON_POINT(%rip), %rax
	movq	%rax, %rdi
	call	al_destroy_bitmap
	movq	CON_POINT+8(%rip), %rax
	movq	%rax, %rdi
	call	al_destroy_bitmap
	movq	BLOCKTYPE(%rip), %rax
	movq	%rax, %rdi
	call	al_destroy_bitmap
	movq	BLOCKTYPE+8(%rip), %rax
	movq	%rax, %rdi
	call	al_destroy_bitmap
	movq	BLOCKTYPE+16(%rip), %rax
	movq	%rax, %rdi
	call	al_destroy_bitmap
	movq	BLOCKTYPE+24(%rip), %rax
	movq	%rax, %rdi
	call	al_destroy_bitmap
	movq	BLOCKTYPE+32(%rip), %rax
	movq	%rax, %rdi
	call	al_destroy_bitmap
	movq	BLOCKTYPE+40(%rip), %rax
	movq	%rax, %rdi
	call	al_destroy_bitmap
	movq	BLOCKTYPE+48(%rip), %rax
	movq	%rax, %rdi
	call	al_destroy_bitmap
	movq	BLOCKTYPE+56(%rip), %rax
	movq	%rax, %rdi
	call	al_destroy_bitmap
	movq	BLOCKTYPE+64(%rip), %rax
	movq	%rax, %rdi
	call	al_destroy_bitmap
	movq	BLOCKTYPE+72(%rip), %rax
	movq	%rax, %rdi
	call	al_destroy_bitmap
	movq	BLOCKTYPE+80(%rip), %rax
	movq	%rax, %rdi
	call	al_destroy_bitmap
	movq	BLOCKTYPE+88(%rip), %rax
	movq	%rax, %rdi
	call	al_destroy_bitmap
	movq	BLOCKTYPE+96(%rip), %rax
	movq	%rax, %rdi
	call	al_destroy_bitmap
	movq	BLOCKTYPE+104(%rip), %rax
	movq	%rax, %rdi
	call	al_destroy_bitmap
	movq	BLOCKTYPE+112(%rip), %rax
	movq	%rax, %rdi
	call	al_destroy_bitmap
	movq	BLOCKTYPE+120(%rip), %rax
	movq	%rax, %rdi
	call	al_destroy_bitmap
	movq	BLOCKTYPE+128(%rip), %rax
	movq	%rax, %rdi
	call	al_destroy_bitmap
	movq	BLOCKTYPE+136(%rip), %rax
	movq	%rax, %rdi
	call	al_destroy_bitmap
	movq	BLOCKTYPE+144(%rip), %rax
	movq	%rax, %rdi
	call	al_destroy_bitmap
	movq	BLOCKTYPE+152(%rip), %rax
	movq	%rax, %rdi
	call	al_destroy_bitmap
	movq	NUMBER(%rip), %rax
	movq	%rax, %rdi
	call	al_destroy_bitmap
	movq	NUMBER+8(%rip), %rax
	movq	%rax, %rdi
	call	al_destroy_bitmap
	movq	NUMBER+16(%rip), %rax
	movq	%rax, %rdi
	call	al_destroy_bitmap
	movq	NUMBER+24(%rip), %rax
	movq	%rax, %rdi
	call	al_destroy_bitmap
	movq	NUMBER+32(%rip), %rax
	movq	%rax, %rdi
	call	al_destroy_bitmap
	movq	NUMBER+40(%rip), %rax
	movq	%rax, %rdi
	call	al_destroy_bitmap
	movq	NUMBER+48(%rip), %rax
	movq	%rax, %rdi
	call	al_destroy_bitmap
	movq	NUMBER+56(%rip), %rax
	movq	%rax, %rdi
	call	al_destroy_bitmap
	movq	NUMBER+64(%rip), %rax
	movq	%rax, %rdi
	call	al_destroy_bitmap
	movq	NUMBER+72(%rip), %rax
	movq	%rax, %rdi
	call	al_destroy_bitmap
	movq	-136(%rbp), %rax
	movq	%rax, %rdi
	call	al_destroy_bitmap
	movq	-104(%rbp), %rbx
	testq	%rbx, %rbx
	je	.L69
	movq	%rbx, %rdi
	call	_ZN10block_listD1Ev
	movq	%rbx, %rdi
	call	_ZdlPv
.L69:
	movq	screen(%rip), %rax
	movq	%rax, %rdi
	call	al_destroy_display
	movl	$0, %eax
	movq	-24(%rbp), %rbx
	xorq	%fs:40, %rbx
	je	.L72
	jmp	.L82
.L73:
	movq	%rax, %r12
	movq	%rbx, %rdi
	call	_ZdlPv
	movq	%r12, %rax
	movq	%rax, %rdi
	call	_Unwind_Resume
.LEHE2:
.L82:
	call	__stack_chk_fail
.L72:
	addq	$176, %rsp
	popq	%rbx
	popq	%r12
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE17:
	.globl	__gxx_personality_v0
	.section	.gcc_except_table,"a",@progbits
.LLSDA17:
	.byte	0xff
	.byte	0xff
	.byte	0x1
	.uleb128 .LLSDACSE17-.LLSDACSB17
.LLSDACSB17:
	.uleb128 .LEHB0-.LFB17
	.uleb128 .LEHE0-.LEHB0
	.uleb128 0
	.uleb128 0
	.uleb128 .LEHB1-.LFB17
	.uleb128 .LEHE1-.LEHB1
	.uleb128 .L73-.LFB17
	.uleb128 0
	.uleb128 .LEHB2-.LFB17
	.uleb128 .LEHE2-.LEHB2
	.uleb128 0
	.uleb128 0
.LLSDACSE17:
	.text
	.size	main, .-main
	.section	.rodata
	.align 4
.LC0:
	.long	1065353216
	.align 4
.LC42:
	.long	1138655232
	.align 4
.LC43:
	.long	1125187584
	.align 4
.LC44:
	.long	1136197632
	.align 4
.LC45:
	.long	1112014848
	.align 4
.LC46:
	.long	1136689152
	.align 4
.LC47:
	.long	1116471296
	.align 4
.LC48:
	.long	1136001024
	.align 4
.LC49:
	.long	1116602368
	.align 4
.LC50:
	.long	1135181824
	.align 4
.LC51:
	.long	1110966272
	.align 4
.LC52:
	.long	1135214592
	.align 4
.LC53:
	.long	1111228416
	.align 4
.LC54:
	.long	1121058816
	.align 4
.LC55:
	.long	1117650944
	.align 4
.LC56:
	.long	1117782016
	.align 4
.LC57:
	.long	1141080064
	.align 4
.LC58:
	.long	1140457472
	.align 4
.LC59:
	.long	1140490240
	.align 4
.LC60:
	.long	1141571584
	.align 4
.LC61:
	.long	1141145600
	.align 4
.LC62:
	.long	1141161984
	.align 4
.LC63:
	.long	1109917696
	.align 4
.LC64:
	.long	1091567616
	.align 4
.LC65:
	.long	1099431936
	.align 4
.LC66:
	.long	1117913088
	.align 4
.LC67:
	.long	1112276992
	.align 4
.LC68:
	.long	1122500608
	.align 4
.LC69:
	.long	1118044160
	.align 4
.LC70:
	.long	1125515264
	.align 4
.LC71:
	.long	1122631680
	.align 4
.LC72:
	.long	1121320960
	.align 4
.LC73:
	.long	1125122048
	.align 4
.LC74:
	.long	1115815936
	.align 4
.LC75:
	.long	1127415808
	.align 4
.LC76:
	.long	1123942400
	.align 4
.LC77:
	.long	1111752704
	.align 4
.LC78:
	.long	1132068864
	.align 4
.LC79:
	.long	1125384192
	.align 4
.LC80:
	.long	1128333312
	.align 4
.LC81:
	.long	1134592000
	.align 4
.LC82:
	.long	1133346816
	.align 4
.LC83:
	.long	1082130432
	.align 4
.LC84:
	.long	1141882880
	.align 4
.LC85:
	.long	1145241600
	.align 4
.LC86:
	.long	1141800960
	.align 4
.LC87:
	.long	1143930880
	.align 4
.LC88:
	.long	1110441984
	.align 4
.LC89:
	.long	1143750656
	.align 4
.LC90:
	.long	1143799808
	.align 4
.LC91:
	.long	1143783424
	.align 8
.LC92:
	.long	0
	.long	1078329344
	.align 8
.LC93:
	.long	2576980378
	.long	1070176665
	.align 4
.LC94:
	.long	1045220557
	.align 4
.LC95:
	.long	1073741824
	.align 4
.LC96:
	.long	1143816192
	.align 4
.LC97:
	.long	1141964800
	.align 4
.LC98:
	.long	1141719040
	.align 4
.LC99:
	.long	1143767040
	.align 4
.LC100:
	.long	1141948416
	.align 4
.LC101:
	.long	1141735424
	.ident	"GCC: (Ubuntu 5.4.0-6ubuntu1~16.04.9) 5.4.0 20160609"
	.section	.note.GNU-stack,"",@progbits
