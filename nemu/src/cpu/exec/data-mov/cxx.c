#include "cpu/exec/helper.h"

make_helper(cbw) {
		reg_w(R_AX) = (int16_t)(int8_t)reg_b(R_AL);

		print_asm("cbw");

		return 1;
}

make_helper(cwd) {
		reg_l(R_EAX) = (int32_t)(int16_t)reg_w(R_AX);

		print_asm("cwde");

		return 1;
}

make_helper(cwq) {
		int32_t value = (int32_t)(int16_t)reg_w(R_AX);

		reg_w(R_DX) = (value >> 16) & 0xffff;

		print_asm("cwd");

		return 1;
}

make_helper(cdq) {
		int64_t value = (int64_t)(int32_t)reg_l(R_EAX);

		reg_l(R_EDX) = (value >> 32) & 0xffffffff;

		print_asm("cdq");

		return 1;
}

make_helper(cxd) {
		if (IS16)
				return cbw(eip);
		else
				return cwd(eip);
}

make_helper(cxq) {
		if (IS16)
				return cwq(eip);
		else
				return cdq(eip);
}
