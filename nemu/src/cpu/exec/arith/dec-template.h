#include "cpu/exec/template-start.h"

#define instr dec

static void do_execute () {
	DATA_TYPE result = op_src->val - 1;
	OPERAND_W(op_src, result);

	/* TODO: Update EFLAGS. */
	of = ((MSB(result + 1)) != (MSB(result)));
    zf = (result == 0);
    sf = ((MSB(result)) != 0);
    make_pf(result);

	print_asm_template1();
}

make_instr_helper(rm)
#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(r)
#endif

#include "cpu/exec/template-end.h"
