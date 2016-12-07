#ifndef __FLOAT_H__
#define __FLOAT_H__

#include "trap.h"

typedef int FLOAT;

static inline int F2int(FLOAT a) {
		FLOAT val = ((int)a > 0)?a:-a;
		val >>= 16;

		return ((int)a > 0)?val:-val;
}

static inline FLOAT int2F(int a) {
	FLOAT val = ((int)a > 0)?a:-a;
	nemu_assert(val <= 0xffff);
	val <<= 16;
	return ((int)a > 0)?val:-val;
}

static inline FLOAT F_mul_int(FLOAT a, int b) {
	FLOAT val = ((int)a > 0)?a:-a;
	val *= (b > 0)?b:-b;

	return (((int)a > 0) ^ (b > 0))?-val:val;
}

static inline FLOAT F_div_int(FLOAT a, int b) {
	FLOAT val = ((int)a > 0)?a:-a;
	val /= (b > 0)?b:-b;

	return (((int)a > 0) ^ (b > 0))?-val:val;
}

FLOAT f2F(float);
FLOAT F_mul_F(FLOAT, FLOAT);
FLOAT F_div_F(FLOAT, FLOAT);
FLOAT Fabs(FLOAT);
FLOAT sqrt(FLOAT);
FLOAT pow(FLOAT, FLOAT);

#endif
