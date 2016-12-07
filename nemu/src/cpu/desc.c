#include "cpu/reg.h"

uint32_t lnaddr_read(swaddr_t, int);

void sreg_update(int seg_num) {
		int offset = seg_index(seg_num) * 8;

		SegDesc temp;
        temp.val = lnaddr_read(gdtr.base + offset, 4) + (((long long)(lnaddr_read(gdtr.base + offset + 4, 4)) << 32));
		sbuff(seg_num) = temp;

}
