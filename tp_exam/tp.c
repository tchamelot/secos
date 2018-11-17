/* GPLv2 (c) Airbus */
#include <debug.h>
#include <info.h>
#include <asm.h>
#include <task.h>
#include <scheduler.h>
#include <syscall.h>
#include <seg_init.h>

extern info_t *info;

task_t tasks[2];
volatile uint32_t *counter = (uint32_t*)0xd00000;

volatile uint8_t state = 0;

void __counter__ counter_task()
{
	*counter = 0;
	while(1)
	{
		if(state == 0)
		{
			state = 1;
			(*counter)++;
		}
	}
}

void __printer__ printer_task()
{
	while(1)
	{
		if(state == 1)
		{
			print_counter(counter);
			state = 0;
		}
	}
}

void tp()
{	
	debug("[+] segmentation\n");
	init_segmem();

	debug("[+] Activating interrupts\n");
	force_interrupts_on();

	debug("[+] Initing tasks\n");
	init_task(&tasks[0], (uint32_t)&counter_task, 0x802000, 0x801000, &tasks[1]);
	init_task(&tasks[1], (uint32_t)&printer_task, 0xc02000, 0xc01000, &tasks[0]);
	restore_task(&tasks[0]);
	while(1);
}
