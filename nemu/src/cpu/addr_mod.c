#include "cpu/reg.h"

#define real 0
#define protect 1

int current_sreg = R_DS;
char addr_mod = real;
char addr_pg = 0;
