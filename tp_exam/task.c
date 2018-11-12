#include <string.h>
#include <asm.h>
#include <debug.h>
#include "seg_init.h"
#include "task.h"

void init_task(task_t* task)
{
	task->state = 1;
	//TODO add gdt_add_tss()
	
	task->ts->s0.esp = task->kstack;
	task->ts->s0.ss = d0_sel;
}

void prepare_task(task_t* task)
{
	uint32_t esp;
	
	esp = get_esp();
	set_esp(task->kstack);

	asm volatile(
		"push %0	\n"
		"push %1	\n"
		"pushf		\n"
		"push %2	\n"
		"push %3	\n"
		"push $0	\n"	// For the scheduler
		"push $0	\n"	//
		"push $0	\n"	//
		"push $0	\n"	//
		"push $0	\n"	//
		"push $0	\n"	//
		"push $0	\n"	//
		"push $0	\n"	//
		"push $0	\n"	//
		"push $0	\n"	//
		::
		"i"(d3_sel),
		"m"(task->ustack),
		"i"(c3_sel),
		"r"(task->code)
	);
	task->kstack = get_esp();
	set_esp(esp);
}

void switch_task(task_t* task)
{
	debug("[Launching] %s\n", task->name);
	task->state |= TASK_INIT | TASK_RUNNING;
	task->ts->s0.esp = task->kstack;
	task->ts->s0.ss = d0_sel;
	force_interrupts_on();
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


