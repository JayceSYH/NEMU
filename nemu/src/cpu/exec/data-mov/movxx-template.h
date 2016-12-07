#include "cpu/exec/template-start.h"
#include "cpu/decode/modrm.h"

#if DATA_BYTE == 1

#define instr movs8rw

static void do_execute() {
		ModR_M modr_m;
		modr_m.val = instr_fetch(cpu.eip + 2, 1);

		int16_t value = (int8_t)(uint8_t)op_src->val;
		reg_w(modr_m.reg) = value;

		print_asm("movsbw %s,%s", op_src->str, regsw[modr_m.reg]);
}

make_instr_helper(rm2r);

#undef instr

#define instr movz8rw

static void do_execute() {
		ModR_M modr_m;
		modr_m.val = instr_fetch(cpu.eip + 2, 1);

		uint16_t value = (uint8_t)op_src->val;
		reg_w(modr_m.reg) = value;

		print_asm("movzbw %s,%s", op_src->str, regsw[modr_m.reg]);
}

make_instr_helper(rm2r);

#undef instr

#endif



#if DATA_BYTE == 1

#define instr movs8rl

static void do_execute() {
		ModR_M modr_m;
		modr_m.val = instr_fetch(cpu.eip + 2, 1);

		int32_t value = (int8_t)(uint8_t)op_src->val;
		reg_l(modr_m.reg) = value;

		print_asm("movsbl %s,%s", op_src->str, regsl[modr_m.reg]);
}

make_instr_helper(rm2r);

#undef instr

#define instr movz8rl

static void do_execute() {
		ModR_M modr_m;
		modr_m.val = instr_fetch(cpu.eip + 2, 1);

		uint32_t value = (uint8_t)op_src->val;
		reg_l(modr_m.reg) = value;

		print_asm("movzbl %s,%s", op_src->str, regsl[modr_m.reg]);
}

make_instr_helper(rm2r);

#undef instr

#endif


#if DATA_BYTE == 2

#define instr movs16rl

static void do_execute() {
		ModR_M modr_m;
		modr_m.val = instr_fetch(cpu.eip + 2, 1);

		int32_t value = (int16_t)op_src->val;
		reg_l(modr_m.reg) = value;

		print_asm("movswl %s,%s", op_src->str, regsl[modr_m.reg]);
}

make_instr_helper(rm2r);

#undef instr

#define instr movz16rl

static void do_execute() {
		ModR_M modr_m;
		modr_m.val = instr_fetch(cpu.eip + 2, 1);

		uint32_t value = (uint16_t)op_src->val;
		reg_l(modr_m.reg) = value;

		print_asm("movzwl %s,%s", op_src->str, regsl[modr_m.reg]);
}

make_instr_helper(rm2r);

#undef instr

#endif

#include "cpu/exec/template-end.h"
