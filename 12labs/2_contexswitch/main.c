#include <stdint.h>
#include <stdio.h>
#include "reg.h"
#include "blink.h"
#include "usart.h"
#include "asm_func.h"

#define TASK_NUM 3
#define PSTACK_SIZE_WORDS 1024 //user stack size = 4 kB

uint32_t *psp_array[TASK_NUM];

void setup_systick(uint32_t ticks);

void init_task(unsigned int task_id, uint32_t *task_addr, uint32_t *psp_init)
{
	//init stack frame
	*(psp_init-1) = 0x01000000;				   //xPSR (T bit has to be 1 in Thumb state)
	*(psp_init-2) = (uint32_t)task_addr; //Return Address
	psp_array[task_id] = psp_init-16;	//stack frame: 8 + r4 ~ r11: 8
}

void task0(void)
{
	printf("[Task0] Start in unprivileged thread mode.\r\n\n");
	printf("[Task0] Control: 0x%x \r\n", (unsigned int)read_ctrl());

	blink(LED_BLUE); //should not return
}

void task1(void)
{
	printf("[Task1] Start in unprivileged thread mode.\r\n\n");
	printf("[Task1] Control: 0x%x \r\n", (unsigned int)read_ctrl());

	blink(LED_GREEN); //should not return
}

void task2(void)
{
	printf("[Task2] Start in unprivileged thread mode.\r\n\n");
	printf("[Task2] Control: 0x%x \r\n", (unsigned int)read_ctrl());

	blink(LED_ORANGE); //should not return
}

int main(void)
{
	init_usart1();

	uint32_t user_stacks[TASK_NUM][PSTACK_SIZE_WORDS];

	//init user tasks
	init_task(0, (uint32_t *)task0, user_stacks[0] + PSTACK_SIZE_WORDS);
	init_task(1, (uint32_t *)task1, user_stacks[1] + PSTACK_SIZE_WORDS);
	init_task(2, (uint32_t *)task2, user_stacks[2] + PSTACK_SIZE_WORDS);

	printf("[Kernel] Start in privileged thread mode.\r\n\n");

	printf("[Kernel] Setting systick...\r\n\n");
	setup_systick(168e6 / 8 / 100); //10 ms

	//start user task
	printf("[Kernel] Switch to unprivileged thread mode & start user task0 with psp.\r\n\n");
	start_user((uint32_t *)task0, user_stacks[0]);

	while (1) //should not go here
		;
}

void setup_systick(uint32_t ticks)
{
	// set reload value
	WRITE_BITS(SYST_BASE + SYST_RVR_OFFSET, SYST_RELOAD_23_BIT, SYST_RELOAD_0_BIT, ticks - 1);

	// uses external reference clock
	CLEAR_BIT(SYST_BASE + SYST_CSR_OFFSET, SYST_CLKSOURCE_BIT);

	// enable systick exception
	SET_BIT(SYST_BASE + SYST_CSR_OFFSET, SYST_TICKINT_BIT);

	// enable systick
	SET_BIT(SYST_BASE + SYST_CSR_OFFSET, SYST_ENABLE_BIT);
}

uint32_t *sw_task(uint32_t *psp)
{
	static unsigned int curr_task_id = 0;

	psp_array[curr_task_id] = psp;

	if (++curr_task_id > TASK_NUM - 1)
		curr_task_id = 0;

	return psp_array[curr_task_id];
}
