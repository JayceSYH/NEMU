#include "cpu/exec/template-start.h"

#ifndef MAKE_MOVCC
#define MAKE_MOVCC

#define make_movcc(cc) \
make_helper(concat(concat(concat(mov, cc), _ ), SUFFIX)) {\
	    int ins_len = concat(decode_rm2r_, SUFFIX)(eip + 1);\
			if (COND(cc))\
				OPERAND_W(op_dest, op_src->val);\
		print_asm("cmov" #cc " %%%s,%%%s", op_src->str, op_dest->str);\
		return 1 + ins_len;\
}

#endif

CC_MAKER(make_movcc)

#include "cpu/exec/template-end.h"
