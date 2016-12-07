#include "cpu/exec/template-start.h"

make_helper(concat(cmps_, SUFFIX)) {
    op_dest->val = swaddr_read(reg_l(R_ESI), DATA_BYTE);
    op_src->val = swaddr_read(reg_l(R_EDI), DATA_BYTE);

	DATA_TYPE result = op_dest->val - op_src->val;

	reg_l(R_ESI) += (df)?(-DATA_BYTE):DATA_BYTE;
	reg_l(R_EDI) += (df)?(-DATA_BYTE):DATA_BYTE;

    of = (SD != SS && SS == SR)?1:0;
    cf = ((SD && SS && SR) || (!SD && SS) || (!SD && !SS && SR))?1:0;
    sf = SR?1:0;
    zf = result?0:1;
	make_pf(result);

	print_asm("cmps" str(SUFFIX) " %%es:(%%edi),%%ds:(%%esi)");

	return 1;
}

#include "cpu/exec/template-end.h"
