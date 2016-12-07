#include "cpu/exec/template-start.h"

#define instr add

static void do_execute () {
	DATA_TYPE result = op_src->val + op_dest->val;

   of = (SD == SS && SD != SR)?1:0;
   cf = ((SD && SS) || ((SD||SS) && !SR))?1:0;
   sf = SR?1:0;
   zf = result?0:1;
   make_pf(result);

	OPERAND_W(op_dest, result);
	print_asm_template2();
}

make_instr_helper(i2a)
make_instr_helper(i2rm)
#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(si2rm)
#endif
make_instr_helper(r2rm)
make_instr_helper(rm2r)

#include "cpu/exec/template-end.h"
