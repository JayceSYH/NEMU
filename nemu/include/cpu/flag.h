#ifndef _FLAG_H_
#define _FLAG_H_

#define print_eflags \
		printf("cf:%d\npf:%d\nzf:%d\nsf:%d\nof:%d\naf:%d\nif:%d\ndf:%d\n", cf, pf, zf, sf, of, af, If, df); \
		fflush(stdout);

#include "cpu/reg.h"
#define cf cpu.eflags.CF
#define pf cpu.eflags.PF
#define zf cpu.eflags.ZF
#define sf cpu.eflags.SF
#define If cpu.eflags.IF
#define df cpu.eflags.DF
#define of cpu.eflags.OF
#define af cpu.eflags.AF

#define eflags_val cpu.eflags.val

#define make_pf(num) char temp = (num & (num >> 4)) & 0x0f; \
                     temp = temp & (temp >> 2); \
					 pf = (temp & (temp >> 1)) & 0x1;

#endif
