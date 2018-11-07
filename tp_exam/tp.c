/* GPLv2 (c) Airbus */
#include <debug.h>
#include <info.h>
#include <string.h>
#include <asm.h>
#include "seg_init.h"
#include "pag_init.h"
#include "task.h"
#include "scheduler.h"

extern info_t *info;
extern tss_t TSS1;
extern tss_t TSS2;

task_t tasks[2];

void __user1__ user1()
//void user1()
{
	debug("[Task: user1] begin\n");

	while(1)
		;//debug("[User1] iddle...\n");
}

//void __user2__ user2()
void user2()
{
	debug("[Task: user2] begin\n");

	while(1);
}

void tp()
{	
	tasks[0].code = (uint32_t*)&user1;
	tasks[0].state = 0;
	tasks[0].ts = &TSS1;
	tasks[0].ts_sel.raw = ts1_sel;
	tasks[0].kstack = 0x802000;
	tasks[0].ustack = 0x801000;
	
	tasks[1].code = (uint32_t*)&user2;
	tasks[1].state = 0;
	tasks[1].ts = &TSS2;
	tasks[1].ts_sel.raw = ts2_sel;
	tasks[1].kstack = 0xc02000;
	tasks[1].ustack = 0xc01000;


	debug("[+] segmentation\n");
	init_segmem();
	//init_paging();

	debug("[+] init user 1\n");
	//init_task(&tasks[0]);
	//init_task(&tasks[1]);
	init_scheduler();
	debug("[+] Activating interrupts\n");
	force_interrupts_on();
	switch_task(&tasks[0]);
	while(1);
}
