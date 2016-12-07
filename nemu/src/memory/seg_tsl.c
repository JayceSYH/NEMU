#include "cpu/reg.h"
#include "common.h"
#include "memory/addr_mod.h"


lnaddr_t segment_translate(swaddr_t addr, int len, int current_sreg) {
		if (addr_mod == real) 
				return addr;

		lnaddr_t gdt = buff_base(current_sreg);
		//assert(buff_limit(current_sreg) == 0xfffff || (addr + len <= buff_limit(current_sreg)));

		return addr + gdt;
}
