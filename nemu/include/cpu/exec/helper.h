#ifndef __EXEC_HELPER_H__
#define __EXEC_HELPER_H__

#include "cpu/helper.h"
#include "cpu/decode/decode.h"

#define IS16 (ops_decoded.is_data_size_16)

#define make_helper_v(name) \
	make_helper(concat(name, _v)) { \
		return (ops_decoded.is_data_size_16 ? concat(name, _w) : concat(name, _l)) (eip); \
	}

#define do_execute concat4(do_, instr, _, SUFFIX)

#define make_instr_helper(type) \
	make_helper(concat5(instr, _, type, _, SUFFIX)) { \
		return idex(eip, concat4(decode_, type, _, SUFFIX), do_execute); \
	}

extern char assembly[];
#ifdef DEBUG
#define print_asm(...) Assert(snprintf(assembly, 80, __VA_ARGS__) < 80, "buffer overflow!")
#else
#define print_asm(...)
#endif

#define print_asm_template1() \
	print_asm(str(instr) str(SUFFIX) " %s", op_src->str)

#define print_asm_template2() \
	print_asm(str(instr) str(SUFFIX) " %s,%s", op_src->str, op_dest->str)

#define print_asm_template3() \
	print_asm(str(instr) str(SUFFIX) " %s,%s,%s", op_src->str, op_src2->str, op_dest->str)

#define COND(cc) concat(cc, _COND)

#define a_COND (cf == 0 && zf == 0) 
#define ae_COND (cf == 0) 
#define b_COND (cf == 1)  
#define be_COND (cf == 1 || zf == 1)  
#define e_COND (zf == 1) 
#define g_COND (zf == 0 && sf == of) 
#define ge_COND (sf == of) 
#define l_COND (sf != of) 
#define le_COND (zf == 1 || sf != of) 
#define ne_COND (zf == 0)  
#define no_COND (of == 0)  
#define np_COND (pf == 0) 
#define ns_COND (sf == 0) 
#define o_COND (of == 1) 
#define p_COND (pf == 1) 
#define s_COND (sf == 1) 
#define na_COND be_COND

#define CC_MAKER(MAKER) \
MAKER(a);\
MAKER(ae);\
MAKER(b);\
MAKER(be);\
MAKER(e);\
MAKER(g);\
MAKER(ge);\
MAKER(l);\
MAKER(le);\
MAKER(ne);\
MAKER(no);\
MAKER(np);\
MAKER(ns);\
MAKER(o);\
MAKER(p);\
MAKER(s);\
MAKER(na);

#define MAKE_CC_HELPER() \
CC_MAKER(CC_HELPER)

#define MAKE_CC_HELPER_V() \
CC_MAKER(CC_HELPER_V)

#define MAKE_CC_HELPER_B() \
CC_MAKER(CC_HELPER_B)

#define CC_HELPER(cc) \
FUNC_MAKER(concat(FUNC, cc))

#define CC_HELPER_V(cc) \
FUNC_MAKER(concat3(FUNC, cc, _v))

#define CC_HELPER_B(cc) \
FUNC_MAKER(concat3(FUNC, cc, _b))

#endif
