#include "cpu/exec/helper.h"
#include "memory/addr_mod.h"
#include "cpu/reg.h"

extern void tlb_init();

make_helper(mov_cr2r) {
		char modr_m = instr_fetch(eip + 1, 1);
		char spr_name[10];
		int reg_num = modr_m & 0x7;
		int spr_num = (modr_m & 0x38) >> 3;
		switch (spr_num) {
				case 0:reg_l(reg_num) = cr0.val;strcpy(spr_name, "%cr0");break;
				case 3:reg_l(reg_num) = cr3.val;strcpy(spr_name, "%cr3");break;
				default:assert(0);
		}

		print_asm("mov %s,%%%s", spr_name, regsl[reg_num]);
		return 2;
}

make_helper(mov_r2cr) {
		char modr_m = instr_fetch(eip + 1, 1);
		char spr_name[10];
		int reg_num = modr_m & 0x7;
		int spr_num = (modr_m & 0x38) >> 3;
		switch (spr_num) {
				case 0:cr0.val = reg_l(reg_num);strcpy(spr_name, "%cr0");addr_mod = cr0.pe;
				if(cr0.pe && cr0.pg) {
						addr_pg = 1;
						cpu.eip += 0xc0000000;
				}
				else {
						cpu.eip &= 0x3fffffff;
						addr_pg = 0;
				}
				break;
				case 3:cr3.val = reg_l(reg_num);strcpy(spr_name, "%cr3");tlb_init();break;
				default:assert(0);
		}

		print_asm("mov %%%s,%s", regsl[reg_num], spr_name);
		return 2;
}

#define DATA_BYTE 2
#include "cpu/exec/template-start.h"
make_helper(mov_rm2seg_w) {
		char modr_m = instr_fetch(eip + 1, 1);
		int seg_num = (modr_m & 0x38) >> 3;
		int ins_len = 1;
		decode_rm_w(eip + 1);

		sreg(seg_num) = op_src->val;

		sreg_update(seg_num);

		print_asm("mov %s,%%%s", op_src->str, sregs[seg_num]);


		return 1 + ins_len;
}

make_helper(mov_seg2rm_w) {
		char modr_m = instr_fetch(eip + 1, 1);
		int ins_len = 1;
		int seg_num = (modr_m & 0x38) >> 3;
		uint16_t seg = sreg(seg_num);

		decode_rm_w(eip + 1);
		OPERAND_W(op_src, seg);

		print_asm("mov %%%s,%%%s",  sregs[seg_num], op_src->str);

		return 1 + ins_len;
}
#include "cpu/exec/template-end.h"
#undef DATA_BYTE
