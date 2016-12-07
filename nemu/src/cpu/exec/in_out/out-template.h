#include "cpu/exec/template-start.h"
#include "device/port-io.h"

make_helper(concat(out_a2i_, SUFFIX)) {
		uint8_t imm = instr_fetch(eip + 1, 1);

		pio_write(imm, DATA_BYTE, REG(R_EAX));

		print_asm("out %%%s,*0x%x", REG_NAME(R_EAX), imm);

		return 2;
}

make_helper(concat(out_a2d_, SUFFIX)) {
		pio_write(reg_w(R_DX), DATA_BYTE, REG(R_EAX));

		print_asm("out %%%s,(%%dx)", REG_NAME(R_EAX));
		
		return 1;
}

#include "cpu/exec/template-end.h"
