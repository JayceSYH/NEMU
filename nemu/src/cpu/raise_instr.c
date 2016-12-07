#include <setjmp.h>
#include "common.h"
#include "cpu/reg.h"
#include "memory/memory.h"
#define IDTE_LEN 8

extern jmp_buf jbuf;
//extern lnaddr_t segment_translate(swaddr_t,int,int);

void raise_instr(uint8_t NO) {	
		//push ..
		reg_l(R_ESP) -= 4;
		swaddr_write(reg_l(R_ESP), 4, eflags_val);

		reg_l(R_ESP) -= 4;
		swaddr_write(reg_l(R_ESP), 4, sreg(R_CS));

		reg_l(R_ESP) -= 4;
		swaddr_write(reg_l(R_ESP), 4, cpu.eip);
		
		If = 0;
		//...
		lnaddr_t base = idtr.base;
		lnaddr_t gate_addr = base + NO * IDTE_LEN;

		unsigned long long temp = lnaddr_read(gate_addr, 4);
		temp += ((unsigned long long)lnaddr_read(gate_addr + 4, 4)) << 32;
		GateDesc gd;
		gd.val = temp;

		sreg(R_CS) = gd.segment;
		sreg_update(R_CS);

		swaddr_t offset = gd.offset_15_0 + (gd.offset_31_16 << 16);

		//hwaddr_t hwaddr = segment_translate(offset, 1, R_CS);

		cpu.eip = offset;

		longjmp(jbuf, 1);
}
