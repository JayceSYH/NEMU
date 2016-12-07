#include "cpu/exec/template-start.h"
#include "memory/addr_mod.h"

make_helper(concat(movstr_, SUFFIX)) {
		if (addr_mod == real)
				SET_SREG(R_DS);
		DATA_TYPE val = swaddr_read(reg_l(R_ESI), DATA_BYTE);
		SET_SREG(R_ES);
		swaddr_write(reg_l(R_EDI), DATA_BYTE, val);

		reg_l(R_ESI) += (df)?(-DATA_BYTE):DATA_BYTE;
		reg_l(R_EDI) += (df)?(-DATA_BYTE):DATA_BYTE;

		print_asm("movs" str(SUFFIX) " %%ds:(%%esi),%%es:(%%edi)");

		return 1;
}

#include "cpu/exec/template-end.h"
