#include "cpu/tlb.h"
#include "cpu/reg.h"
#include "memory/addr_mod.h"
#include "monitor/monitor.h"

extern int nemu_state;

#define TLBE_NUM 64
//#define TLBE_NUM 0

#if TLBE_NUM == 64
#define TLB_MASK 0x3f
#elif TLBE_NUM == 128
#define TLB_MASK 0x7f
#endif

#define MAKE_TLB_FIND(i) \
if (tlb[i].tag == addr_tag && tlb[i].valid_bit) {\
		last_index_4 = last_index_3;\
		last_index_3 = last_index_2;\
		last_index_2 = last_index;\
		last_index = i;\
		return (addr & 0xfff) + (tlb[i].pte.page_frame << 12);\
}

#define MAKE_TLB_FIND_8(i) \
MAKE_TLB_FIND(i)\
MAKE_TLB_FIND(i + 1)\
MAKE_TLB_FIND(i + 2)\
MAKE_TLB_FIND(i + 3)\
MAKE_TLB_FIND(i + 4)\
MAKE_TLB_FIND(i + 5)\
MAKE_TLB_FIND(i + 6)\
MAKE_TLB_FIND(i + 7)

#define MAKE_TLB_FIND_64(i) \
MAKE_TLB_FIND_8(i)\
MAKE_TLB_FIND_8(i + 8)\
MAKE_TLB_FIND_8(i + 16)\
MAKE_TLB_FIND_8(i + 24)\
MAKE_TLB_FIND_8(i + 32)\
MAKE_TLB_FIND_8(i + 40)\
MAKE_TLB_FIND_8(i + 48)\
MAKE_TLB_FIND_8(i + 56)

static TLBE tlb[TLBE_NUM];
extern uint32_t cache_read_l1(hwaddr_t, size_t);
extern uint32_t hwaddr_read(hwaddr_t, size_t);

static uint32_t random_() {
		static uint32_t rand = 58236;
		rand = rand * 25681 + 19546543;
		return rand;
}


void tlb_init() {
		int i = 0;
		for (; i < TLBE_NUM; ++i) {
				tlb[i].valid_bit = 0;
		}
}

static int tlb_replace(uint32_t pte_val, uint32_t addr) {
		int i = 0;
		for (; i < TLBE_NUM; ++i) {
				if (!tlb[i].valid_bit) break;
		}

		i = (i == TLBE_NUM)?(random_() & TLB_MASK):i;

		tlb[i].pte.val = pte_val;
		tlb[i].valid_bit = 1;
		tlb[i].tag = (addr >> 12) & 0xfffff;

		return i;
}



uint32_t tlb_find(lnaddr_t addr) {
		static int last_index = 0;
		static int last_index_2 = 0;
		static int last_index_3 = 0;
		static int last_index_4 = 0;

		uint32_t addr_tag = (addr >> 12) & 0xfffff;

		if (tlb[last_index].tag == addr_tag && tlb[last_index].valid_bit) 
				return (tlb[last_index].pte.page_frame << 12) + (addr & 0xfff);

		if (tlb[last_index_2].tag == addr_tag && tlb[last_index_2].valid_bit) 
				return (tlb[last_index_2].pte.page_frame << 12) + (addr & 0xfff);

		if (tlb[last_index_3].tag == addr_tag && tlb[last_index_3].valid_bit) 
				return (tlb[last_index_3].pte.page_frame << 12) + (addr & 0xfff);

		if (tlb[last_index_4].tag == addr_tag && tlb[last_index_4].valid_bit) 
				return (tlb[last_index_4].pte.page_frame << 12) + (addr & 0xfff);
		//int i = 0;

/*
		for (; i < TLBE_NUM; ++i) {
				
				if (tlb[i].tag == addr_tag && tlb[i].valid_bit) {
						printf("%d\n", i);
						last_index_3 = last_index_2;
						last_index_2 = last_index;
						last_index = i;
						return (addr & 0xfff) + (tlb[i].pte.page_frame << 12);
				}
				
		}
*/

		MAKE_TLB_FIND_64(0);

		hwaddr_t pde_addr = pd_base + ((addr >> 22) & 0x3ff) * 4;

		PDE pde;
		pde.val = hwaddr_read(pde_addr, 4);
		lnaddr_t pt_base = pde.page_frame << 12;

		hwaddr_t pte_addr = pt_base + ((addr >> 12) & 0x3ff) * 4;

		PTE pte;
		pte.val = hwaddr_read(pte_addr, 4);

		if (!pte.present) {
				printf("PageFault at 0x%x\n", buff_base(R_CS) + addr);
				nemu_state = STOP;
				return 0;
		}

		last_index = tlb_replace(pte.val, addr);

		return (pte.page_frame << 12) + (addr & 0xfff);
}
