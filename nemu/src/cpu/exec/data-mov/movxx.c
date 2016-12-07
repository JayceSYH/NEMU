#include "cpu/exec/helper.h"
#include"common.h"

#define DATA_BYTE 1
#include "movxx-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "movxx-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "movxx-template.h"
#undef DATA_BYTE

make_helper(movzx8_rm2r_v) {
		if (IS16) 
				return movz8rw_rm2r_b(eip);
		else
				return movz8rl_rm2r_b(eip);
}


make_helper(movsx8_rm2r_v) {
		if (IS16) 
				return movs8rw_rm2r_b(eip);
		else
				return movs8rl_rm2r_b(eip);
}

inline make_helper(movsx16_rm2r_l) {
		return movs16rl_rm2r_w(eip);
}

inline make_helper(movzx16_rm2r_l) {
		return movz16rl_rm2r_w(eip);
}
