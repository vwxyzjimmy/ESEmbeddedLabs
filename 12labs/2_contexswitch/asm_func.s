.syntax unified

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

.type systick_handler, %function
.global systick_handler
systick_handler:
	//save lr (EXC_RETURN)
	//??????
  push  {lr}

	//save r4-r11 to user stack
	mrs	r0,	psp
	stmdb	r0!,	{r4-r11}

	//pass psp of curr task by r0 and get psp of the next task
	bl	sw_task
	//psp of the next task is now in r0

	//load the next task
	ldmia	r0!,	{r4-r11}
	msr	psp,	r0

	//restore lr (EXC_RETURN)
	//??????
  pop {lr}
	bx	lr
