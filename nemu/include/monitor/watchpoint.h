#ifndef __WATCHPOINT_H__
#define __WATCHPOINT_H__

#include "common.h"

typedef struct watchpoint {
	int NO;
	struct watchpoint *next;

	/* TODO: Add more members if necessary */
	int state;
	char exp[40];
	uint32_t saved_value;


} WP;

void init_wp_list();
int add_wp(char *str);
void delete_wp(int no);
int query_wp(swaddr_t);
void print_wp();

#endif
