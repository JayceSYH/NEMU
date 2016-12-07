#include "cpu/exec/helper.h"
#include "cpu/reg.h"

make_helper(std) {
		df = 1;
		print_asm("std");
		return 1;
}

make_helper(sti) {
		If = 1;
		print_asm("sti");
		return 1;
}

make_helper(stc) {
		cf = 1;
		print_asm("stc");
		return 1;
}
