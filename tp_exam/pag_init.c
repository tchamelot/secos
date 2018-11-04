#include <debug.h>
#include <pagemem.h>
#include <cr.h>
#include "pag_init.h"

void init_paging(void)
{
	int i;
	uint32_t cr0;

	pde32_t *pgd_kernel = (pde32_t*)0x00400000;
	pte32_t *ptb_kernel = (pte32_t*)0x00401000;
	pte32_t *ptb_paging = (pte32_t*)0x00402000;

	pde32_t *pgd1 = (pde32_t*)0x00403000;
	pte32_t *ptb1 = (pte32_t*)0x00404000;

	pde32_t *pgd2 = (pde32_t*)0x004050000;
	pte32_t *ptb2 = (pte32_t*)0x00406000;

	debug("[+] paging\n");

	// Kernel paging
	for(i = 0; i < 1024; i++)
		pg_set_entry(&ptb_kernel[i], PG_KRN|PG_RW, i);

	memset((void*)ptb_paging, 0, PAGE_SIZE);
	for(i = 0; i < 7; i++)
		pg_set_entry(&ptb_paging[i], PG_KRN|PG_RW, pt32_idx(0x00400000) + i);

	memset((void*)pgd_kernel, 0, PAGE_SIZE);
	pg_set_entry(&pgd_kernel[0], PG_KRN|PG_RW, page_nr(ptb_kernel));
	//pg_set_entry(&pgd_kernel[960], PG_KRN|PG_RW, page_nr(ptb_paging));

	// User 1 paging
	memset((void*)ptb1, 0, PAGE_SIZE);
	pg_set_entry(&ptb1[0], PG_USR|PG_RW, pt32_idx(BASE_STACK_USR1));	// user 1 stack	4kb
	pg_set_entry(&ptb1[1], PG_KRN|PG_RW, pt32_idx(BASE_KSTACK_USR1));	// user 1 kernel stack 4kb
	pg_set_entry(&ptb1[2], PG_USR|PG_RO, pt32_idx(BASE_CODE_USR1));		// user 1 code 4kb
	pg_set_entry(&ptb1[3], PG_USR|PG_RW, pt32_idx(BASE_SHARE_MEM));		// user 1 share mem 4kb

	memset((void*)pgd1, 0, PAGE_SIZE);
	pg_set_entry(&pgd1[2], PG_USR|PG_RW, page_nr(ptb1));

	// User 2 paging
	memset((void*)ptb2, 0, PAGE_SIZE);
	pg_set_entry(&ptb2[0], PG_USR|PG_RW, pt32_idx(BASE_STACK_USR2));	// user 2 stack	4kb
	pg_set_entry(&ptb2[1], PG_KRN|PG_RW, pt32_idx(BASE_KSTACK_USR2));	// user 2 kernel stack 4kb
	pg_set_entry(&ptb2[2], PG_USR|PG_RO, pt32_idx(BASE_CODE_USR2));		// user 2 code 4kb
	pg_set_entry(&ptb2[3], PG_USR|PG_RO, pt32_idx(BASE_SHARE_MEM));		// user 2 share mem 4kb

	memset((void*)pgd2, 0, PAGE_SIZE);
	pg_set_entry(&pgd2[0], PG_USR|PG_RW, page_nr(ptb2));
	
	debug("\tsetting cr3\n");
	set_cr3((uint32_t)pgd_kernel);
	cr0 = get_cr0();
	debug("\tenabling paging\n");
	set_cr0(cr0|CR0_PG);
}
