#include "cpu/exec/template-start.h"

make_helper(concat(ret_near_, SUFFIX)) {
#if DATA_BYTE == 2
		cpu.eip = swaddr_read(reg_l(R_ESP), 2) - 1;
		reg_l(R_ESP) += 2;
		cpu.eip &= 0x0000ffff;
#else
		cpu.eip = swaddr_read(reg_l(R_ESP), 4) - 1;
		reg_l(R_ESP) += 4;
#endif
		print_asm("ret");
		return 1;
}

make_helper(concat(ret_neari_, SUFFIX)) {
#if DATA_BYTE == 2
		cpu.eip = swaddr_read(reg_l(R_ESP), 2) - 3;
		reg_l(R_ESP) += 2;
		cpu.eip &= 0x0000ffff;
#else
		cpu.eip = swaddr_read(reg_l(R_ESP), 4) - 3;
		reg_l(R_ESP) += 4;
#endif
		int16_t imm = instr_fetch(eip + 1, 2);
		reg_l(R_ESP) += imm;
		print_asm("ret");
		return 3;
}

#include "cpu/exec/template-end.h"
