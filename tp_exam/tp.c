/* GPLv2 (c) Airbus */
#include <debug.h>
#include <info.h>
#include <string.h>
#include "seg_init.h"
#include "pag_init.h"
#include "task.h"

extern info_t *info;
extern tss_t TSS1;

void __user1__ user1()
//void user1()
{
	debug("[Task: user1] begin\n");

	while(1);
}

//void __user2__ user2()
void user2()
{
	debug("[Task: user2] begin\n");

	while(1);
}

void tp()
{	
	task_t t_user1;
	t_user1.code = (uint32_t*)&user1;
	t_user1.state = 0;
	t_user1.ts = &TSS1;
	t_user1.ts_sel.raw = ts1_sel;
	t_user1.kstack = 0x802000;
	t_user1.ustack = 0x801000;

	debug("[+] segmentation\n");
	init_segmem();
	//init_paging();

	debug("[+] init user 1\n");
	//init_task(&t_user1, TSS1, (uint16_t)ts1_sel, (uint32_t*)user1, 0x802000, 0x801000);
	init_task(&t_user1);
}
