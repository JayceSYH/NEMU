#include "cpu/exec/helper.h"
#include "cpu/decode/modrm.h"

uint32_t lnaddr_read(lnaddr_t, size_t);

make_helper(lgdt) {
		int ins_len;
		hwaddr_t addr;
		ModR_M modr_m;
		
		modr_m.val = instr_fetch(eip + 1, 1);
		ins_len = load_addr(eip + 1, &modr_m, op_src);
		addr = op_src->addr;

		uint16_t limit = swaddr_read(addr, 2);
		lnaddr_t gdt_addr = swaddr_read(addr + 2, 4);
		
		gdtr.limit = limit;
		gdtr.base = gdt_addr;
		
		print_asm("lgdt %s", op_src->str);
		
		return 1 + ins_len;
}

make_helper(lidt) {		
		int ins_len;
		hwaddr_t addr;
		ModR_M modr_m;

		modr_m.val = instr_fetch(eip + 1, 1);
		ins_len = load_addr(eip + 1, &modr_m, op_src);
		addr = op_src->addr;

		uint16_t limit = swaddr_read(addr, 2);
		lnaddr_t idt_addr = swaddr_read(addr + 2, 4);

		idtr.limit = limit;
		idtr.base = idt_addr;

		print_asm("lidt %s", op_src->str);

		return 1 + ins_len;
}


