#include <debug.h>
#include <pagemem.h>
#include <cr.h>
#include <asm.h>
#include <task.h>
#include <pag_init.h>

extern uint32_t __kernel_start__, __kernel_end__;

/**
 * \fn				uint32_t new_page()
 * \brief			Allocate new 4ko memory zone for ptb or pgd. 
 */
uint32_t new_page()
{
	static uint32_t current_page = BASE_PAGE_ADDR;

	if(current_page > LAST_PAGE_ADDR)
	{
		panic("Fatal error : Kernel cannot allocate more page!\n");
	}

	current_page += PAGE_SIZE;

	return current_page;
}

void enable_paging()
{
	uint32_t cr0 = get_cr0();
	set_cr0(cr0|CR0_PG);
}

/**
 * \fn 				void map_address(pde32_t* pgd, uint32_t begin, uint32_t end, uint8_t opt)
 * \brief 			Identity map from address begin to address end with the option opt
 *
 * \param pgd 		A page directory allready initialized
 * \param begin 	The begining address of the zone to map
 * \param end		The end address of the zone to map
 * \param opt		The options for the zone mapped
 */
void map_address(pde32_t* pgd, uint32_t begin, uint32_t end, uint8_t opt)
{
	uint32_t pde_end_idx = pd32_idx(end);
	uint32_t j = pt32_idx(begin);

	for(uint32_t i = pd32_idx(begin); i <= pde_end_idx; i++)
	{
		pte32_t* pte;

		// Check if pte already exist
		if(pgd[i].p == 1)
			pte = (pte32_t*)(pgd[i].addr << 12);
		else
			pte = (pte32_t*)new_page();
		
		// Check if the address is in the same pd32_idx and if the address is below the end address
		while((pd32_idx(i * 0x400000 + j * 0x1000) <= i) && ((i * 0x400000 + j * 0x1000) <= end))
		{
			pg_set_entry(&pte[j], opt, page_nr(i * 0x400000 + j * 0x1000));
			j++;
		}
		j = 0;
		pg_set_entry(&pgd[i], opt, page_nr(pte));
	}
}

/**
 * \fn 				void map_shared_memory(pde32_t* pgd, uint32_t addr_phys, uint32_t addr_virt, uint8_t opt)
 * \brief 			Map a shared memory zone of 4ko at a specific virtual address
 *
 * \param pgd		A page directory allready initialized
 * \param addr_phys	The physical address of the shared memory zone
 * \param addr_virt	The virtual address of the shared memory zone
 * \param opt		The options for the zone mapped
 */
void map_shared_memory(pde32_t* pgd, uint32_t addr_phys, uint32_t addr_virt, uint8_t opt)
{
	pte32_t* pte;
	uint32_t pde = pd32_idx(addr_virt);

	if(pgd[pde].p == 1)
			pte = (pte32_t*)(pgd[pde].addr << 12);
		else
			pte = (pte32_t*)new_page();

	pg_set_entry(&pte[pt32_idx(addr_virt)], opt, page_nr(addr_phys));
	pg_set_entry(&pgd[pde], opt, page_nr(pte));
}

/**
 * \fn				void init_page_task(task_t* task, uint32_t task_start, uint32_t task_end)
 * \brief			Init paging for a new task
 *
 * \param task		The task to initialize
 * \param task_start	The start address of the task's code
 * \param task_end	The end address of the task's code
 */
void init_page_task(task_t* task, uint32_t task_start, uint32_t task_end)
{

	pde32_t* pgd = (pde32_t*)new_page();

	// Map kernel
	map_address(pgd, (uint32_t)&__kernel_start__-0x2000, (uint32_t)&__kernel_end__, PG_KRN|PG_RW);

	// Map page
	map_address(pgd, BASE_PAGE_ADDR, LAST_PAGE_ADDR, PG_KRN|PG_RW);

	// Map kernel stack
	map_address(pgd, task_start - 0x1000, task_start -1, PG_KRN|PG_RW);

	// Map task code
	map_address(pgd, task_start, task_end, PG_USR|PG_RO);

	// Map task stack
	map_address(pgd, task_start - 0x2000, task_start - 0x1fff, PG_USR|PG_RW);

	// Map shared memory
	map_shared_memory(pgd, 0xd00000, task_start + 0x4000, PG_USR|PG_RW);

	task->cr3 = (uint32_t)pgd;
}
