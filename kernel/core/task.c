#include <string.h>
#include <asm.h>
#include <debug.h>
#include <gpr.h>
#include <task.h>
#include <seg_init.h>
#include <pag_init.h>

extern tss_t TSS;

void init_task(task_t* task, uint32_t code,
	uint32_t kstack, uint32_t ustack, task_t* next_task)
{
	memset(task, 0, sizeof(task_t));

	task->ctx.eip.raw = code;
	task->ctx.cs.raw = c3_sel;
	task->ctx.eflags.raw = get_flags();
	task->ctx.esp.raw = ustack;
	task->ctx.ss.raw = d3_sel;

	task->kstack = kstack;
	task->state = 0;

	task->next_task = next_task;
}

void save_task(task_t* task, int_ctx_t* ctx)
{
	task->kstack = (uint32_t)(ctx) + 60;
	memcpy(&task->ctx, ctx, sizeof(int_ctx_t));	
}

void __regparm__(1) restore_task(task_t* task)
{
	TSS.s0.esp = task->kstack;
	//set_cr3(task->cr3);
	set_esp(task->kstack);
	asm volatile(
		"push %0 \n" // ss
		"push %1 \n" // esp
		"push %2 \n" // eflags
		"push %3 \n" // cs
		"push %4 \n" // eip
		"push %5 \n" // eax
		"push %6 \n" // ecx
		"push %7 \n" // edx
		"push %8 \n" // ebx
		"push %9 \n" // esp
		"push %10\n" // ebp
		"push %11\n" // esi
		"push %12\n" // edi
		//"mov %%eax, %13	\n"
		//"mov %%cr3, %%eax\n"
		"popa	 \n" // restore registers
		"iret    \n"
		::
		"g"(task->ctx.ss),
		"g"(task->ctx.esp),
		"g"(task->ctx.eflags),
		"g"(task->ctx.cs),
		"g"(task->ctx.eip),
		"g"(task->ctx.gpr.eax.raw),
		"g"(task->ctx.gpr.ecx.raw),
		"g"(task->ctx.gpr.edx.raw),
		"g"(task->ctx.gpr.ebx.raw),
		"g"(task->ctx.gpr.esp.raw),
		"g"(task->ctx.gpr.ebp.raw),
		"g"(task->ctx.gpr.esi.raw),
		"g"(task->ctx.gpr.edi.raw)
		//"g"(task->cr3)
	);
}
