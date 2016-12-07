#include "cpu/exec/template-start.h"

make_helper(concat(push_m_, SUFFIX)) {
		//swaddr_t addr = instr_fetch(eip + 1, 4);
		//DATA_TYPE value = swaddr_read(addr, DATA_BYTE);
		int ins_len = concat(decode_r2rm_, SUFFIX)(eip + 1);
		reg_l(4) -= DATA_BYTE;
		swaddr_write(reg_l(4), DATA_BYTE, (DATA_TYPE)op_dest->val);

        print_asm("push %s", op_dest->str);

		return 1 + ins_len;
}

make_helper(concat(push_i_, SUFFIX)) {
		DATA_TYPE value = instr_fetch(eip + 1, DATA_BYTE);
		reg_l(4) -= DATA_BYTE;
		swaddr_write(reg_l(4), DATA_BYTE, value);

        print_asm("push $0x%x", value);

		return 1 + DATA_BYTE;
}

make_helper(concat(push_r_, SUFFIX)) {
		char ins = instr_fetch(eip, 1);
		int reg_num = ins - 0x50;
		DATA_TYPE value = REG(reg_num);
		reg_l(R_ESP) -= DATA_BYTE;
		swaddr_write(reg_l(R_ESP), DATA_BYTE, value);

		print_asm("push %%%s",REG_NAME(reg_num));
		
		return 1;
}
#include "cpu/exec/template-end.h"
