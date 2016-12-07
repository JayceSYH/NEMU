#include "cpu/exec/template-start.h"
#include "device/port-io.h"

make_helper(concat(in_i2a_, SUFFIX)) {
		uint8_t imm = instr_fetch(eip + 1, 1);

		REG(R_EAX) = pio_read(imm, DATA_BYTE);

		print_asm("in *0x%x,%%%s", imm, REG_NAME(R_EAX));

		return 2;
}

make_helper(concat(in_d2a_, SUFFIX)) {
		REG(R_EAX) = pio_read(reg_w(R_DX), DATA_BYTE);

		print_asm("in (%%dx),%%%s", REG_NAME(R_EAX));

		return 1;
}



#include "cpu/exec/template-end.h"
