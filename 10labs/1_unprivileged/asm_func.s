.syntax unified

.global	read_sp
read_sp:
	mov r0,	sp ;
	bx lr	;

.global	read_msp
read_msp:
	MRS r0, msp	;
	isb
	bx lr	;

.global	read_psp
read_psp:
	MRS r0, psp	;
	isb
	bx lr	;

.global	read_ctrl
read_ctrl:
	MRS r0, CONTROL ;
	isb
	bx lr	;

.global	start_user
start_user:
	mov lr, r0	;
	MSR psp, r1 ;
	mov r1, #0b11;
	MSR CONTROL, r1;
	isb
	bx lr	;

.global	sw_priv
sw_priv:
	mov r1,  0;
	MSR CONTROL, r1 ;
	isb
	bx lr	;
