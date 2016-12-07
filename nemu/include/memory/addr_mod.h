#ifndef _ADDR_MOD_
#define _ADDR_MOD_

enum { real, protect };

extern int current_sreg;
extern char addr_mod;
extern char addr_pg;

#define SET_SREG(sreg)  current_sreg = sreg 

#endif
