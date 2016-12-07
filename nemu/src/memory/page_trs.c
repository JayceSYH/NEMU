#include "memory/addr_mod.h"
#include "cpu/reg.h"
#include "common.h"
#include "memory/tlb.h"

uint32_t page_translate(lnaddr_t addr) {
		if (addr_mod == real || !addr_pg) return addr;
		return tlb_find(addr);
}
