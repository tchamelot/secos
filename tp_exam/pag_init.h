#ifndef _PAG_INIT_H_
#define _PAG_INIT_H_

#include <pagemem.h>
#include <cr.h>

#define PGD_KRN			0xf0000000
#define PGD_USR1		0xf0002000;
#define PGD_USR2		0xf0004000;

#define BASE_STACK_USR1		0x00800000
#define BASE_KSTACK_USR1	0x00801000
#define BASE_CODE_USR1		0x00802000

#define BASE_STACK_USR2		0x00c00000
#define BASE_KSTACK_USR2	0x00c01000
#define BASE_CODE_USR2		0x00c02000

#define BASE_SHARE_MEM		0x01000000
void init_paging(void);

#endif
