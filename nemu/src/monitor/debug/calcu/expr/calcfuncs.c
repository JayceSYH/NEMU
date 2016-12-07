#include <math.h>
#include <stdlib.h>
#include <memory/memory.h>
#include "calcfuncs.h"


make_func2(f_add) {
		*para2 += *para1;
}

make_func2(f_minus) {
		*para2 -= *para1;
}

make_func2(f_multi) {
		*para2 *= *para1;
}

make_func2(f_divid) {
		*para2 /= *para1;
}

make_func2(f_xor) {
		*para2 |= *para1;
}

make_func2(f_rs) {
		*para2 >>= *para1;
}

make_func2(f_ls) {
		*para2 <<= *para1;
}

make_func2(f_eq) {
		*para2 = (*para2 == *para1);
}

make_func2(f_ne) {
		*para2 = (*para2 != *para1);
}

make_func2(f_l) {
		*para2 = (*para2 < *para1);
}

make_func2(f_g) {
		*para2 = (*para2 > *para1);
}

make_func2(f_ge) {
		*para2 = (*para2 >= *para1);
}

make_func2(f_le) {
		*para2 = (*para2 <= *para1);
}

make_func2(f_and) {
		*para2 = (*para2 && *para1);
}

make_func2(f_ands) {
		*para2 |= *para1;
}
make_func2(f_ors) {
		*para2 |= *para1;
}

make_func2(f_or) {
		*para2 = (*para2 || *para1);
}

make_func2(f_mod) {
		*para2 %= *para1;
}

make_func1(f_pos) {
}

make_func1(f_neg) {
		*para = -*para;
}

make_func1(f_dp) {
		*para = swaddr_read(*para, 4);
}

make_func1(f_not) {
		*para = !*para;
}

make_func1(f_nots) {
		*para = ~*para;
}
