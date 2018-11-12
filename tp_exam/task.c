#include <string.h>
#include <asm.h>
#include <debug.h>
#include "seg_init.h"
#include "task.h"

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
