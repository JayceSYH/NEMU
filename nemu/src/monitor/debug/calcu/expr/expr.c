#include "expr.h"
#include "calcfuncs.h"
#include <stdlib.h>
#include <stdio.h>

int ParaNum(char type) {
		switch(type) {
				case '+': case '-':
				case '*': case '/':
				case '^': case '|':
				case '&': case AND:
				case OR : case EQ:
				case NE:  case RS:
				case LS:  case G:
				case L:   case GE:
				case LE:  case '%':
				return 2;

				case NEG: case POS:
				case DP: case '~':
				case '!':
				return 1;

				default :return 0;
		}
}

void *GetFunc(char type) {
		switch (type) {
				case '+':return f_add;
				case '-':return f_minus;
				case '*':return f_multi;
				case '/':return f_divid;
				case '^':return f_xor;
				case '|':return f_ors;
				case '&':return f_ands;
				case '%':return f_mod;
				case AND:return f_and;
				case OR:return f_or;
				case EQ:return f_eq;
				case NE:return f_ne;
				case LS:return f_ls;
				case RS:return f_rs;
				case G:return f_g;
				case L:return f_l;
				case GE:return f_ge;
				case LE:return f_le;
				case NEG:return f_neg;
				case POS:return f_pos;
				case '~':return f_nots;
				case '!':return f_not;
				default :exit(-1);
		}
}

unsigned calcu(Token *tokens) {
		unsigned buff[100];
		int buff_top = 0;

		int no = 0;

		while (tokens[no].type != TOKEN_END) {
				if (TOKEN_NUM(tokens[no])) {
						buff[buff_top] = tokens[no].value;
						buff_top++;
						++no;
						continue;
				}//if num

				if (ParaNum(tokens[no].type) == 1) 
						(*(FP1)GetFunc(tokens[no].type))(&buff[buff_top - 1]);
				else {
						(*(FP2)GetFunc(tokens[no].type))(&buff[buff_top - 1], &buff[buff_top - 2]);
						buff_top--;
				}

				no++;
		}

#if _DEBUF_ == ON
		assert(buff_top == 1);
#endif

		return buff[0];
}
