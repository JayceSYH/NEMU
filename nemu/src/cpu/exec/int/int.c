#include "cpu/exec/helper.h"
#include "common.h"
#include "cpu/reg.h"
int pusha(swaddr_t eip);
extern char suffix;
void raise_instr(uint8_t);

make_helper(int_i8) {
		uint8_t NO = instr_fetch(eip + 1, 1);

		print_asm("int %d", NO);

		cpu.eip += 2;
		raise_instr(NO);

		return 2;
}

make_helper(int4) {
		print_asm("int 4");

		if (of) {
				cpu.eip += 1;
				raise_instr(4);
		}

		return 1;
}

make_helper(iret) {
		cpu.eip = swaddr_read(reg_l(R_ESP), 4) - 1;
		reg_l(R_ESP) += 4;

		sreg(R_CS) = swaddr_read(reg_l(R_ESP), 4);
		sreg_update(R_CS);
		reg_l(R_ESP) += 4;

		eflags_val = swaddr_read(reg_l(R_ESP), 4);
		reg_l(R_ESP) += 4;

		print_asm("iret");

		return 1;
}

		
