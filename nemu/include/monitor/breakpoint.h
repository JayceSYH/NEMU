#ifndef __UI_BREAKPOINT_H__
#define __UI_BREAKPOINT_H__

#include "common.h"

#define INT3_CODE 0xcc

typedef struct breakpoint {
	int NO;
	unsigned Position;
	unsigned char Temp_instr;
	struct breakpoint *next;

	/*  Add more members if necessary */


} BP;

void print_bp();
BP* search_bp(int);
void free_bp(BP*);
BP* new_bp();

#endif
