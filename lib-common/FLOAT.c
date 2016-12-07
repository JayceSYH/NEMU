#include "FLOAT.h"

FLOAT Fabs(FLOAT a) {
	return (((int)a) > 0)?a:-a;
}

FLOAT F_mul_F(FLOAT a, FLOAT b) {
    unsigned long long mul = Fabs(a);
	mul *= Fabs(b);
	mul >>= 16;
	nemu_assert(!(mul & 0xffffffff00000000));
	return (((int)a > 0) ^ ((int)b > 0))?-(FLOAT)mul:(FLOAT)mul;
}

FLOAT F_div_F(FLOAT a, FLOAT b) {
    unsigned long long a_ = Fabs(a);
    unsigned long long b_ = Fabs(b);
	int shift = 16;
	while (!((int)a_ <= 0 || (int)b_ <= 0)) {
			a_ <<= 1;
			shift --;
	}

	FLOAT div = ((unsigned)a_ / (unsigned)b_) << shift;
	return (((int)a > 0) ^ ((int)b > 0))?-div:div;
}

FLOAT f2F(float a) {
	unsigned b = *(unsigned*)&a;
	unsigned val = (b & 0x7fffff) | 0x800000;
	char index = ((b & 0x7f800000) >> 23) - 127 - 7;
	val = (index > 0)?(val << index):(val >> (-index));

	return (b & 0x80000000)?-val:val;
}


FLOAT sqrt(FLOAT x) {
	FLOAT dt, t = int2F(2);

	do {
		dt = F_div_int((F_div_F(x, t) - t), 2);
		t += dt;
	} while(Fabs(dt) > f2F(1e-4));

	return t;
}

FLOAT pow(FLOAT x, FLOAT y) {
	/* we only compute x^0.333 */
	FLOAT t2, dt, t = int2F(2);
		
	do {
		t2 = F_mul_F(t, t);
		dt = (F_div_F(x, t2) - t) / 3;
		t += dt;
	} while(Fabs(dt) > f2F(1e-3));

	return t;
}

