#include "cpu/exec/template-start.h"


#define make_setcc(cc)\
make_helper(concat(set, cc)) {\
		int ins_len = concat(decode_r2rm_, SUFFIX)(eip + 1);\
\
		OPERAND_W(op_dest, COND(cc));\
		print_asm("set" str(cc) " %s", op_dest->str);\
		return 1 + ins_len;\
}

CC_MAKER(make_setcc)

#include "cpu/exec/template-end.h"
