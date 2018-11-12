/* GPLv2 (c) Airbus */
#include <debug.h>
#include <info.h>
#include <string.h>
#include <asm.h>
#include "seg_init.h"
#include "pag_init.h"
#include "task.h"
#include "scheduler.h"
#include "syscall.h"

extern info_t *info;

task_t tasks[2];

uint8_t mutex = 0;

uint32_t* counter = (uint32_t*)0xd00000;

void __user1__ user1()
{
	uint8_t i = 0;
	while(1)
	{
		if(i++  == 0)
		{	
			while(mutex != 0);
			mutex++;
			(*counter)++;
			mutex--;
		}
	}
}

void user2()
{
	uint16_t i = 0;
	while(1)
	{
		if(i++ == 0)
		{
			while(mutex != 0);
			mutex++;
			print_counter(counter);
			mutex--;
		}
	}
}

void tp()
{	
	tasks[0].code = (uint32_t*)&user1;
	tasks[0].state = 0;
	tasks[0].kstack = 0x802000;
	tasks[0].ustack = 0x801000;
	tasks[0].next_task = &tasks[1];
	
	tasks[1].code = (uint32_t*)&user2;
	tasks[1].state = 0;
	tasks[1].kstack = 0xc02000;
	tasks[1].ustack = 0xc01000;
	tasks[1].next_task = &tasks[0];

	(*counter) = 0;

	debug("[+] segmentation\n");
	init_segmem();
	//init_paging();

	init_syscall();
	init_scheduler();
	debug("[+] Activating interrupts\n");
	force_interrupts_on();
	prepare_task(&tasks[0]);
	prepare_task(&tasks[1]);
	force_scheduling();
	while(1);
}
