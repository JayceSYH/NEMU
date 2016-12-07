#include "cpu/exec/helper.h"

make_helper(pushad) {
		reg_l(R_ESP) -= 4;
		swaddr_write(reg_l(R_ESP), 4, reg_l(0));
		reg_l(R_ESP) -= 4;
		swaddr_write(reg_l(R_ESP), 4, reg_l(1));
		reg_l(R_ESP) -= 4;
		swaddr_write(reg_l(R_ESP), 4, reg_l(2));
		reg_l(R_ESP) -= 4;
		swaddr_write(reg_l(R_ESP), 4, reg_l(3));
		reg_l(R_ESP) -= 4;
		swaddr_write(reg_l(R_ESP), 4, reg_l(4));
		reg_l(R_ESP) -= 4;
		swaddr_write(reg_l(R_ESP), 4, reg_l(5));
		reg_l(R_ESP) -= 4;
		swaddr_write(reg_l(R_ESP), 4, reg_l(6));
		reg_l(R_ESP) -= 4;
		swaddr_write(reg_l(R_ESP), 4, reg_l(7));

        print_asm("pushad");

		return 1;
}

