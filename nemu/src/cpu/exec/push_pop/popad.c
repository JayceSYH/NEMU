#include "cpu/exec/helper.h"

make_helper(popad) {
		reg_l(7) = swaddr_read(reg_l(4), 4);
		reg_l(R_ESP) += 4;
		reg_l(6) = swaddr_read(reg_l(4), 4);
		reg_l(R_ESP) += 4;
		reg_l(5) = swaddr_read(reg_l(4), 4);
		reg_l(R_ESP) += 4;
		reg_l(4) = swaddr_read(reg_l(4), 4);
		reg_l(R_ESP) += 4;
		reg_l(3) = swaddr_read(reg_l(4), 4);
		reg_l(R_ESP) += 4;
		reg_l(2) = swaddr_read(reg_l(4), 4);
		reg_l(R_ESP) += 4;
		reg_l(1) = swaddr_read(reg_l(4), 4);
		reg_l(R_ESP) += 4;
		reg_l(0) = swaddr_read(reg_l(4), 4);
		reg_l(R_ESP) += 4;

        print_asm("popad");

		return 1;
}

