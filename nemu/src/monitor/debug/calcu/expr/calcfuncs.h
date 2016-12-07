#ifndef _CALFUNC_
#define _CALFUNC_

#include "expr.h"

#define make_func1(funcname) \
		void funcname(unsigned *para)

#define make_func2(funcname) \
		void funcname(unsigned *para1, unsigned *para2)

make_func2(f_add);
make_func2(f_minus);
make_func2(f_multi);
make_func2(f_divid);
make_func2(f_xor);
make_func2(f_rs);
make_func2(f_ls);
make_func2(f_eq);
make_func2(f_ne);
make_func2(f_g);
make_func2(f_l);
make_func2(f_ge);
make_func2(f_le);
make_func2(f_and);
make_func2(f_or);
make_func2(f_ands);
make_func2(f_mod);
make_func2(f_ors);
make_func1(f_pos);
make_func1(f_neg);
make_func1(f_dp);
make_func1(f_not);
make_func1(f_nots);

#endif
