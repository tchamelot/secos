#ifndef _SHCEDULER_H_
#define _SHCEDULER_H_

#include <segmem.h>
#include <intr.h>


void init_scheduler();
void scheduler_irq();
void force_scheduling();

#endif
