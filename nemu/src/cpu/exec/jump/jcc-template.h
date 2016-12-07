#include "cpu/exec/template-start.h"

#if DATA_BYTE == 1
#define INSTR_LEN 1
#else
#define INSTR_LEN 2
#endif

#define make_jcc(cc) \
make_helper(concat4(j, cc, _, SUFFIX)) {\
		int ins_len = concat(decode_i_, SUFFIX)(eip + 1);\
        if (COND(cc)) {\
              cpu.eip += (int32_t)(DATA_TYPE_S)op_src->val;\
               print_asm("j" str(cc) " %x", cpu.eip + INSTR_LEN + ins_len);\
               }\
		else \
				print_asm("j" str(cc) " %x", cpu.eip + (int32_t)(DATA_TYPE_S)op_src->val + INSTR_LEN + ins_len);\
\
		return 1 + ins_len;\
}

CC_MAKER(make_jcc)

#if DATA_BYTE == 1

#define cxz_COND (reg_w(R_CX) == 0)
#define ecxz_COND (reg_l(R_ECX) == 0)

make_jcc(cxz)
make_jcc(ecxz)

#undef cxz_COND
#undef ecxz_COND

#endif

#undef INSTR_LEN

#include "cpu/exec/template-end.h"
