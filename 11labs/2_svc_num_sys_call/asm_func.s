.syntax unified

.global	read_sp
read_sp:
	mov	r0,	sp
	bx	lr

.global	read_msp
read_msp:
	mrs	r0,	msp
	bx	lr

.global	read_psp
read_psp:
	mrs	r0,	psp
	bx	lr

.global	read_ctrl
read_ctrl:
	mrs	r0,	control
	bx	lr

.global	start_user
start_user:
	movs	lr,	r0
	msr	psp,	r1

	movs	r3,	#0b11
	msr	control,	r3
	isb

	bx	lr

.global	write_ctrl
write_ctrl:
	msr	control,	r0

	bx	lr

.global	sys_call
sys_call:
	SVC	#0x0
	bx	lr

.global	sys_call_add
	sys_call_add:
		SVC	#0xA
		bx	lr

.global	sys_call_write
	sys_call_write:
		SVC	#0x4
		bx	lr

.type svc_handler, %function
.global svc_handler

svc_handler:
	movs	r0,	lr
	mrs	r1,	msp
	mrs	r2,	control

	movs	r3,	#0b0
	msr	control,	r3
	isb

	b	svc_handler_c
