#ifndef _PAG_INIT_H_
#define _PAG_INIT_H_

#include <pagemem.h>
#include <cr.h>
#include <task.h>

#define BASE_PAGE_ADDR 0x400000
#define LAST_PAGE_ADDR 0x4ff000

uint32_t new_page();
void enable_paging();
void map_address(pde32_t* pde, uint32_t begin, uint32_t end, uint8_t opt);
void init_page_task(task_t* task, uint32_t task_start, uint32_t task_end);
uint8_t is_paging_enable();
#endif
