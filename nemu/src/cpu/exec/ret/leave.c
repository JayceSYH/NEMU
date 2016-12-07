#include "cpu/exec/helper.h"

make_helper(leave) {
		if (IS16) 	{
				reg_w(R_SP) = reg_w(R_BP);
				reg_w(R_BP) = swaddr_read(reg_l(R_ESP), 2);
				reg_w(R_SP) += 2;
		}
		else {
				reg_l(R_ESP) = reg_l(R_EBP);
				reg_l(R_EBP) = swaddr_read(reg_l(R_ESP), 4);
				reg_l(R_ESP) += 4;
		}

		print_asm("leave");
		return 1;
}
