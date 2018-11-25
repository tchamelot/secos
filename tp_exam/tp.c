/* GPLv2 (c) Airbus */
#include <debug.h>
#include <info.h>
#include <asm.h>
#include <task.h>
#include <syscall.h>
#include <seg_init.h>
#include <pag_init.h>

extern info_t *info;
extern int __counter_start__, __counter_end__;
extern int __printer_start__, __printer_end__;


task_t tasks[2];

void __counter__ counter_task()
{	
	uint32_t *counter = (uint32_t*)0xd00000;

	*counter = 0;

	while(1)
	{
		(*counter)++;
	}
}

void __printer__ printer_task()
{
	uint32_t *counter = (uint32_t*)0xd00000;

	while(1)
	{
		print_counter(counter);
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
	init_page_task(&tasks[0], (uint32_t)&__counter_start__, (uint32_t)&__counter_end__);
	
	init_task(&tasks[1], (uint32_t)&printer_task, 0xc02000, 0xc01000, &tasks[0]);
	init_page_task(&tasks[1], (uint32_t)&__printer_start__, (uint32_t)&__printer_end__);
	
	set_cr3(tasks[0].cr3);
	enable_paging();

	restore_task(&tasks[0]);
	while(1);
}
