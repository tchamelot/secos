#include <task.h>
#include <seg_init.h>

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

void restore_task(task_t* task)
{
	TSS.s0.esp = task->kstack;
	switch_task(task);
}
