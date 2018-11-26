#include <task.h>
#include <seg_init.h>

extern tss_t TSS;

/**
 * \fn				void init_task(task_t* task, uint32_t code, uint32_t kstack, uint32_t ustack, task_t* next_task)
 * \brief 			Initialize new task
 *
 * \param task		The task to initialize
 * \param kstack	The kernel stack bottom address
 * \param ustack	The user stack bottom address
 * \param next_task	The task to schedule after this one
 */
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

	task->next_task = next_task;
}

/**
 * \fn				void save_task(task_t* task, int_ctx_t* ctx)
 * \brief			Save the current task's state
 *
 * \param task		The task to save
 * \param ctx		The current task's state
 */
void save_task(task_t* task, int_ctx_t* ctx)
{
	task->kstack = (uint32_t)(ctx) + 60;
	memcpy(&task->ctx, ctx, sizeof(int_ctx_t));	
}

/**
 * \fn				void restore_task(task_t* task)
 * \brief			Restore and resume the task
 *
 * \param task		The task to restore
 */
void restore_task(task_t* task)
{
	TSS.s0.esp = task->kstack;
	switch_task(task);
}
