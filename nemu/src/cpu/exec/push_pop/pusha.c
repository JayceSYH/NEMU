#include "cpu/exec/helper.h"

make_helper(pusha) {
		reg_l(R_ESP) -= 2;
		swaddr_write(reg_l(4), 2, reg_w(0));
		reg_l(R_ESP) -= 2;
		swaddr_write(reg_l(4), 2, reg_w(1));
		reg_l(R_ESP) -= 2;
		swaddr_write(reg_l(4), 2, reg_w(2));
		reg_l(R_ESP) -= 2;
		swaddr_write(reg_l(4), 2, reg_w(3));
		reg_l(R_ESP) -= 2;
		swaddr_write(reg_l(4), 2, reg_w(4));
		reg_l(R_ESP) -= 2;
		swaddr_write(reg_l(4), 2, reg_w(5));
		reg_l(R_ESP) -= 2;
		swaddr_write(reg_l(4), 2, reg_w(6));
		reg_l(R_ESP) -= 2;
		swaddr_write(reg_l(4), 2, reg_w(7));

        print_asm("pusha");

		return 1;
}

