/* GPLv2 (c) Airbus */
#include <debug.h>
#include <info.h>
#include <cr.h>
#include <pagemem.h>

extern info_t *info;

void show_cr3()
{
	cr3_reg_t cr3 = {.raw = get_cr3()};
	debug("CR3 : %p\n", cr3.raw);
}

void enable_paging()
{
	cr0_reg_t cr0 = {.raw = get_cr0()};
	cr0.raw |= CR0_PG;
	set_cr0(cr0);
}

void init_paging()
{
	int i;
	uint32_t* target = (uint32_t*)0xc0000000;
	char* str_1 = (char*)0x700000;
	char* str_2 = (char*)0x7ff000;
	pde32_t* pgd = (pde32_t*)0x600000;
	pte32_t* ptb = (pte32_t*)0x601000;
	pte32_t* ptb2 = (pte32_t*)0x602000;
	pte32_t* ptb3 = (pte32_t*)0x603000;

	for(i=0;i<1024;i++)
		pg_set_entry(&ptb[i], PG_KRN|PG_RW, i);

	memset((void*)pgd, 0, PAGE_SIZE);
	pg_set_entry(&pgd[0], PG_KRN|PG_RW, page_nr(ptb));

	for(i=0;i<1024;i++)
		pg_set_entry(&ptb2[i], PG_KRN|PG_RW, i+1024);
	
	pg_set_entry(&pgd[1], PG_KRN|PG_RW, page_nr(ptb2));

	memset((void*)ptb3, 0, PAGE_SIZE);
	pg_set_entry(&ptb3[0], PG_KRN|PG_RW, page_nr(pgd));
	pg_set_entry(&pgd[0x300], PG_KRN|PG_RW, page_nr(ptb3));

	pg_set_entry(&ptb2[pt32_idx(0x700000)], PG_KRN|PG_RW, page_nr(0x2000));
	pg_set_entry(&ptb2[pt32_idx(0x7ff000)], PG_KRN|PG_RW, page_nr(0x2000));

	set_cr3((uint32_t)(pgd));
	enable_paging();

	debug("PTB entry : %p\n",ptb3[0].raw);
	debug("PGD[0] = %p | target = %p\n", pgd[0], *target);
	debug("%p : %s | %p : %s\n", str_1, str_1, str_2, str_2);
}

void tp()
{
	init_paging();
	show_cr3();
}
