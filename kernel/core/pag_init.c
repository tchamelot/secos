#include <debug.h>
#include <pagemem.h>
#include <cr.h>
#include <asm.h>
#include <task.h>
#include <pag_init.h>

extern uint32_t __kernel_start__, __kernel_end__;

static uint32_t CURRENT_PAGE = BASE_PAGE_ADDR;

uint32_t new_page()
{
	uint32_t current_page = CURRENT_PAGE;
	if(current_page > LAST_PAGE_ADDR)
	{
		debug("Fatal error : Kernel cannot allocate more page!\n");
		while(1)
			halt();
	}
	CURRENT_PAGE += 0x1000;
	return current_page;
}

void enable_paging()
{
	uint32_t cr0 = get_cr0();
	set_cr0(cr0|CR0_PG);
}

uint8_t is_paging_enable()
{
	return ((get_cr0() & CR0_PG) == CR0_PG);
}

void map_address(pde32_t* pde, uint32_t begin, uint32_t end, uint8_t opt)
{
	uint32_t pde_end_idx = pd32_idx(end);
	uint32_t j = pt32_idx(begin);

	for(uint32_t i = pd32_idx(begin); i <= pde_end_idx; i++)
	{
		pte32_t* pte;

		// Check if pte already exist
		if(pde[i].p == 1)
			pte = (pte32_t*)(pde[i].addr << 12);
		else
			pte = (pte32_t*)new_page();
		
		// Check if the address is in the same pd32_idx and if the address is below the end address
		while((pd32_idx(i * 0x400000 + j * 0x1000) <= i) && ((i * 0x400000 + j * 0x1000) <= end))
		{
			pg_set_entry(&pte[j], opt, page_nr(i * 0x400000 + j * 0x1000));
			j++;
		}
		j = 0;
		pg_set_entry(&pde[i], opt, page_nr(pte));
	}
}

void init_page_task(task_t* task, uint32_t task_start, uint32_t task_end)
{

	debug("d:%x, d:%x\n", task_start, task_end);
	pde32_t* pde = (pde32_t*)new_page();

	// Map kernel
	map_address(pde, (uint32_t)&__kernel_start__-0x2000, (uint32_t)&__kernel_end__, PG_KRN|PG_RW);

	// Map page
	map_address(pde, BASE_PAGE_ADDR, LAST_PAGE_ADDR, PG_KRN|PG_RW);

	// Map kernel stack
	map_address(pde, task_start - 0x1000, task_start, PG_KRN|PG_RW);

	// Map task code
	map_address(pde, task_start, task_end, PG_USR|PG_RO);

	// Map task stack
	map_address(pde, task_start - 0x2000, task_start - 0x1fff, PG_USR|PG_RW);

	// Map shared memory
	map_address(pde, 0xd00000, 0xd00000 + 0x1000, PG_USR|PG_RW);

	task->cr3 = (uint32_t)pde;
}
