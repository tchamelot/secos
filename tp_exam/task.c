#include <string.h>
#include "seg_init.h"
#include "task.h"

void init_task(task_t* task)
{
	task->state = 1;
	//TODO add gdt_add_tss()
	
	task->ts->s0.esp = task->kstack;
	task->ts->s0.ss = d0_sel;
}

void switch_task(task_t* task)
{
	task->state |= TASK_INIT | TASK_RUNNING;
	task->ts->s0.esp = task->kstack;
	task->ts->s0.ss = d0_sel;
	set_tr(task->ts_sel);
	
	asm volatile(
		"push %0	\n"
		"push %1	\n"
		"pushf		\n"
		"push %2	\n"
		"push %3	\n"
		"iret"
		::
		"i"(d3_sel),
		"m"(task->ustack),
		"i"(c3_sel),
		"r"(task->code)
	);
}
