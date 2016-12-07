#include "cpu/exec/template-start.h"
#include "memory/addr_mod.h"

make_helper(concat(stos_, SUFFIX)) {
		DATA_TYPE val = REG(R_EAX);

		SET_SREG(R_ES);
		swaddr_write(reg_l(R_EDI), DATA_BYTE, val);

		reg_l(R_EDI) += (df)?(-DATA_BYTE):DATA_BYTE;

		print_asm("stos %%%s,%%es:(%%edi)", REG_NAME(R_EAX));

		return 1;
}

#include "cpu/exec/template-end.h"
