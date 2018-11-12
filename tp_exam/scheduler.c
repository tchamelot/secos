#include <debug.h>
#include <segmem.h>
#include <gpr.h>
#include <intr.h>
#include "scheduler.h"
#include "seg_init.h"
#include "task.h"

extern task_t tasks[2];
static task_t* current_task = tasks;

void init_scheduler()
{
	idt_reg_t idtr;
	
	debug("[+] init scheduler\n");

	get_idtr(idtr);

	idtr.desc[32].offset_1 = (uint16_t)((uint32_t)scheduler_irq);
	idtr.desc[32].offset_2 = (uint16_t)(((uint32_t)scheduler_irq)>>16);
}

void scheduler_irq()
{
	int_ctx_t* ctx;

	// Je préfaire le jijutsu ^^
	asm volatile("leave");
	
	// Prepare access to int_ctx_t
	asm volatile("pushl $-1");
	asm volatile("push $32");

	// Prepare next PIT irq
	asm volatile("push %eax");
	asm volatile("movb $0x20, %al");
	asm volatile("outb %al, $0x20");
	asm volatile("pop %eax");

	asm volatile("pusha");
	ctx = (int_ctx_t*)get_esp();
	
	// Scheduling	
	if((ctx->cs.raw & 0x3) != 0)
	{
		current_task->kstack = get_esp();
		
		current_task->next_task->ts->s0.esp = current_task->next_task->kstack + 60;
		set_esp(current_task->next_task->kstack);
		current_task = current_task->next_task;
	}

	// Return to task
	asm volatile("popa");
	asm volatile("add $8, %esp");
	asm volatile("iret");
}

void force_scheduling()
{
	current_task->next_task->state |= TASK_RUNNING | TASK_INIT;
	current_task->next_task->ts->s0.esp = current_task->next_task->kstack;
	set_tr(current_task->next_task->ts_sel);
	set_esp(current_task->next_task->kstack);
	current_task = current_task->next_task;
	asm volatile("popa");
	asm volatile("add $8, %esp");
	asm volatile("iret");
}

void __regparm__(1) scheduler_handler(int_ctx_t* ctx)
{
	debug("cs : %x\n", ctx->cs.raw);
}
