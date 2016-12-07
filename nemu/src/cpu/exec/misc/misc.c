#include "cpu/exec/helper.h"
#include "cpu/decode/modrm.h"

#include "monitor/breakpoint.h"

make_helper(exec);

make_helper(nop) {
	print_asm("nop");
	return 1;
}

make_helper(int3) {
    static int flag = 1;
	int ins_len = 1;

    if (flag == 1) {
		void do_int3();
		do_int3();
		print_asm("int3");
		cpu.eip -= 1;
	}
	else {
			bool success;
			unsigned char find_bp(swaddr_t, bool *);
			uint8_t opcode = find_bp(eip, &success);
			assert(success);
			swaddr_write(eip, 1, opcode);
			ins_len = exec(eip);
			swaddr_write(eip, 1, 0xcc);
	}

	flag = (flag == 0);

	return ins_len;
}


make_helper(lea) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	int len = load_addr(eip + 1, &m, op_src);
	reg_l(m.reg) = op_src->addr;

	print_asm("leal %s,%%%s", op_src->str, regsl[m.reg]);
	return 1 + len;
}

make_helper(halt) {
		assert(If);

		while (!cpu.INTR) {
		}

		print_asm("halt");

		return 1;
}
