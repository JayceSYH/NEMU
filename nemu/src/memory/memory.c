#include "common.h"
#include "cpu/reg.h"
#include "memory/cache.h"
#include "monitor/monitor.h"
#include "memory/addr_mod.h"

uint32_t segment_translate(swaddr_t, int, int);
uint32_t page_translate(lnaddr_t);
int is_mmio(hwaddr_t);
uint32_t mmio_read(hwaddr_t, size_t, int);
void mmio_write(hwaddr_t, size_t, uint32_t, int);

uint32_t dram_read(hwaddr_t, size_t);
void dram_write(hwaddr_t, size_t, uint32_t);

/* Memory accessing interfaces */

uint32_t hwaddr_read(hwaddr_t addr, size_t len) {
    int map_num;
	if ((map_num = is_mmio(addr)) != -1)
			return mmio_read(addr, len, map_num);
    return cache_read_l1(addr, len);
	//return dram_read(addr, len) & (~0u >> ((4 - len) << 3));
}

void hwaddr_write(hwaddr_t addr, size_t len, uint32_t data) {
    int map_num;
	if ((map_num = is_mmio(addr)) != -1) 
			mmio_write(addr, len, data, map_num);
    else 
			cache_write_l1(addr, len, data);
			//dram_write(addr, len, data);
}

uint32_t lnaddr_read(lnaddr_t addr, size_t len) {
    current_sreg = R_DS;
	int temp = (addr + len) & 0xffffff;
	if (addr_pg && temp != 0 && temp < len) {
			assert(0);
			return 0;
	}
	else {
			hwaddr_t hwaddr = page_translate(addr);
			return hwaddr_read(hwaddr, len);
	}
}

void lnaddr_write(lnaddr_t addr, size_t len, uint32_t data) {
    current_sreg = R_DS;
	uint32_t temp = (addr + len) & 0xffffff;

	if (addr_pg && temp != 0 && temp < len)
			assert(0);
    else {
			hwaddr_t hwaddr = page_translate(addr);
			hwaddr_write(hwaddr, len, data);
	}
}

uint32_t swaddr_read(swaddr_t addr, size_t len) {
#ifdef DEBUG
	assert(len == 1 || len == 2 || len == 4);
#endif
    lnaddr_t lnaddr = segment_translate(addr, len, current_sreg);
    return lnaddr_read(lnaddr, len);
}

void swaddr_write(swaddr_t addr, size_t len, uint32_t data) {
#ifdef DEBUG
	assert(len == 1 || len == 2 || len == 4);
	if (addr_pg)
			if (!((uint32_t)(addr >= 0x100000 || addr & 0xffc00000) > 0 || (addr >= 0xa0000 && addr < (0xa0000 + 64000)))) {
					fprintf(stderr, "segmentation fault at addr 0x%x\n", addr);
					//nemu_state = STOP;
					assert(0);
					return;
			}
#endif

    lnaddr_t lnaddr = segment_translate(addr, len, current_sreg);
	lnaddr_write(lnaddr, len, data);
}

