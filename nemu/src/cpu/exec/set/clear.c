#include "cpu/exec/helper.h"
#include "cpu/flag.h"

make_helper(clc) {
		cf = 0;
		print_asm("clc");
		return 1;
}

make_helper(cld) {
		df = 0;
		print_asm("cld");
		return 1;
}

make_helper(cli) {
		If = 0;
		print_asm("cli");
		return 1;
}
