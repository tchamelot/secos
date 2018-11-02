/* GPLv2 (c) Airbus */
#include <debug.h>
#include <info.h>
#include "seg_init.h"
#include "pag_init.h"

extern info_t *info;

void tp()
{	
	debug("[+] segmentation\n");
	init_segmem();
	debug("[+] paging\n");
	init_paging();

}
