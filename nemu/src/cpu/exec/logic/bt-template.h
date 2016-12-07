#include "cpu/exec/template-start.h"

#define instr bt

static void do_execute() {
		DATA_TYPE result = 1 & (op_dest->val >> (uint8_t)(0xff & op_src->val));
		cf = result;

		print_asm_template2();
}

make_instr_helper(i2rm);
make_instr_helper(r2rm);

#include "cpu/exec/template-end.h"
