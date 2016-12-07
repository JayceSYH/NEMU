#include "cpu/exec/helper.h"

make_helper(popa) {
		reg_w(7) = swaddr_read(reg_l(R_ESP), 2);
		reg_l(R_ESP) += 2;
		reg_w(6) = swaddr_read(reg_l(R_ESP), 2);
		reg_l(R_ESP) += 2;
		reg_w(5) = swaddr_read(reg_l(R_ESP), 2);
		reg_l(R_ESP) += 2;
		reg_w(4) = swaddr_read(reg_l(R_ESP), 2);
		reg_l(R_ESP) += 2;
		reg_w(3) = swaddr_read(reg_l(R_ESP), 2);
		reg_l(R_ESP) += 2;
		reg_w(2) = swaddr_read(reg_l(R_ESP), 2);
		reg_l(R_ESP) += 2;
		reg_w(1) = swaddr_read(reg_l(R_ESP), 2);
		reg_l(R_ESP) += 2;
		reg_w(0) = swaddr_read(reg_l(R_ESP), 2);
		reg_l(R_ESP) += 2;

        print_asm("popa");

		return 1;
}

