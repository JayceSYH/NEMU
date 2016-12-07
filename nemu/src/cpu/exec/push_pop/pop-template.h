#include "cpu/exec/template-start.h"

make_helper(concat(pop_m_, SUFFIX)) {
		int ins_len = concat(decode_r2rm_, SUFFIX)(eip);
		DATA_TYPE value = swaddr_read(reg_l(R_ESP), DATA_BYTE);
		reg_l(R_ESP) += DATA_BYTE;
		OPERAND_W(op_dest, value);

        print_asm("pop %s", op_dest->str);

		return 1 + ins_len;
}

make_helper(concat(pop_r_, SUFFIX)) {
		DATA_TYPE value = swaddr_read(reg_l(4), DATA_BYTE);
		reg_l(R_ESP) += DATA_BYTE;
		unsigned char ins = instr_fetch(eip , 1);
		REG(ins - 0x58) = value;

        print_asm("pop %%%s",REG_NAME(ins - 0x58));

		return 1;
}

#include "cpu/exec/template-end.h"
