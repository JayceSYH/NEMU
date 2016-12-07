#include "cpu/decode/modrm.h"
#include "cpu/exec/template-start.h"

#define instr lea

make_helper(concat(lea_, SUFFIX)) {
		ModR_M modr_m; 
		modr_m.val = instr_fetch(cpu.eip + 1, 1);
		int reg_num = modr_m.reg;
		int ins_len = load_addr(cpu.eip + 1, &modr_m, op_src);
		swaddr_t addr = op_src->addr;
#if DATA_BYTE == 2
		reg_w(reg_num) = addr & 0x0000ffff;
#else 
		reg_l(reg_num) = addr;
#endif
		print_asm("lea" str(SUFFIX) " %s,%%%s", op_src->str, REG_NAME(reg_num));

		return 1 + ins_len;
}



#include "cpu/exec/template-end.h"
