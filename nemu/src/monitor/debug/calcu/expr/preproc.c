#include "expr.h"
#include "memory/memory.h"
#include "cpu/reg.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

extern struct rule rules[];

#define NAME_NUM (sizeof(rules) / sizeof(rules[0]) )
/*
void DestroyTokens(char **tokens, int num) {
		int i = 0;
		for (; i < num; ++i)
				free(tokens[i]);

		free(tokens);
}
*/
int CopyTo(char **dest, char * src) {
		int len = 0;
		char *p = src;

		while (*p != ' ' && *p != 0) {
				++len;
				++p;
		}

		*dest = (char*)malloc(len + 1);

		strncpy(*dest, src, len);

		(*dest)[len] = 0;

		return len;
}

#define search_reg(x) \
for (; i < 8; ++i) { \
		if (0 == strcmp(token->exp + 1, concat(regs, x)[i]))\
				return concat(reg_, x)(i);\
}

unsigned GetNum(Token *token, bool *success) {
#if _DEBUG_ == ON
		assert(str != NULL);
#endif

		*success = true;

//NUM
		if (IS_NUM(token->type)) {
				if (*token->exp == '0' && *(token->exp + 1) == 'x') {
						unsigned val;
						sscanf(token->exp, "%x", &val);
						return val;
				}
				else
						return atoi(token->exp);
		}

//REG
		if (token->type == REG) {
				if (strcmp("$eip", token->exp) == 0)
						return cpu.eip;

				int i = 0;
				if (*(token->exp + 1) == 'e') {
						search_reg(l);
				}
				else {
						char suf = *(token->exp + 2);
						if (suf == 'l' || suf == 'h') {
								search_reg(b);
						}
						else {
								search_reg(w);
						}
				}

				printf("GetNum can't recognize reg: %s\n", token->exp);
		}

//TOKEN
		if (token->type == TOKEN) {
				VarEntry *ve = SearchEntry(token->exp);
				if (ve) {
						return ve->value;
				}
		}

		fprintf(stderr, "GetNum Error:can't find tag of exp: %s\n",token->exp);
		*success = false;

		return 0;
}

void SetPos_Neg(Token *tokens, int num) {
		int i = 0;
		for (; i < num; ++i) {
				if (tokens[i].type != '-' && tokens[i].type != '+') continue;

				if (i == 0 || tokens[i -1].type == '(' || ParaNum(tokens[i - 1].type) > 0) {
						tokens[i].type = (tokens[i].type == '-')?NEG:POS;
				}
		}
}

void SetDP(Token *tokens, int num) {
		int i = 0;
		for (; i < num; ++i) {
				if (tokens[i].type != '*') continue;

				if (i == 0 || tokens[i - 1].type == '(' || ParaNum(tokens[i - 1].type) > 0) {
						tokens[i].type = DP;
				}
		}
}

void SetPriv(Token *tokens, int num) {
		int i = 0;
		for (; i < num; ++i) {
				switch (tokens[i].type) {
						case OR: case AND:
						tokens[i].priv = 1;break;

						case '|': case '&':
						case '^': 
						tokens[i].priv = 2;break;

						case EQ: case NE:
						tokens[i].priv = 3;break;

						case G:  case L:
						case GE: case LE:
						tokens[i].priv = 4;break;

						case RS: case LS:
						tokens[i].priv = 5;break;

						case '+':case '-':
						tokens[i].priv = 6;break;

						case '*':case '/':
						case '%':
						tokens[i].priv = 7;break;

						case '~':case '!':
						tokens[i].priv = 8;break;

						case POS:case NEG:
						tokens[i].priv = 9;break;

						case '(':case ')':
						tokens[i].priv = 10;break;

						//defalut:assert(0);
				}
		}
}

#define GetName(x) (x.exp)

bool CheckPare(Token* tokens) {
		int no = 0;
		int lp = 0;

		while (tokens[no].type != TOKEN_END) {
				if (tokens[no].type == '(') {
						lp ++;
						if (no > 0)
								if (ParaNum(tokens[no - 1].type) == 0 && tokens[no - 1].type != '(') {
										printf("Invalid %s befor '(' (at token %d)\n", GetName(tokens[no - 1]), no + 1);
										return false;
								}

						if (ParaNum(tokens[no + 1].type) == 2 && tokens[no + 1].type != '(') {
								printf("Invalid %s after '(' (at token %d)\n", GetName(tokens[no + 1]), no + 1);

								return false;
						}

						if (tokens[no + 1].type == TOKEN_END) {
								printf("miss tokens after '(' (at token %d)\n", no + 1);
								return false;
						}
				}
				else if (tokens[no].type == ')') {
						lp--;
						if (lp < 0) {
								printf("miss '(' befor ')' (at token %d)\n", no + 1);
								return false;
						}

						if (tokens[no + 1].type != TOKEN_END) 
								if (ParaNum(tokens[no + 1].type) != 2 && tokens[no + 1].type != ')') {
										printf("Invalid %s after ')' (at token %d)\n", GetName(tokens[no + 1]), no + 1);
										return false;
								}
						
						if (!TOKEN_NUM(tokens[no - 1]) && tokens[no - 1].type != ')') {
								printf("Invalid %s befor ')' (at token %d)\n", GetName(tokens[no - 1]), no + 1);
								return false;
						}
				}

				no++;
		}

		if (lp > 0) {
				printf("miss ')' in expression\n");
				
				return false;
		}

		return true;

}

bool CheckOps(Token *tokens) {
		int no = 0;

		while (tokens[no].type != TOKEN_END) {
				if (ParaNum(tokens[no].type) == 1) {
						if (tokens[no + 1].type == TOKEN_END) {
								printf("miss tokens after %s (at token %d)\n", GetName(tokens[no]), no + 1);

								return false;
						}

						if (no > 0) 
								if (TOKEN_NUM(tokens[no - 1])) {
										printf("Invalid %s befor %s (at token %d)\n", GetName(tokens[no - 1]), GetName(tokens[no]), no + 1);
										return false;
								}

						if (ParaNum(tokens[no + 1].type) == 2) {
								printf("Invalid %s after %s (at token %d)\n" , GetName(tokens[no + 1]), GetName(tokens[no]), no + 1);

								return false;
						}
				}
				else if (ParaNum(tokens[no].type) == 2) {
						if (tokens[no + 1].type == TOKEN_END) {
								printf("miss tokens after %s (at token %d)\n", GetName(tokens[no]), no + 1);

								return false;
						}

						if (no == 0 || (!TOKEN_NUM(tokens[no - 1]) && tokens[no - 1].type != ')')) {
								printf("Invalid %s befor %s (at token %d)\n", GetName(tokens[no - 1]), GetName(tokens[no]), no + 1);
								
								return false;
						}

						if (ParaNum(tokens[no + 1].type) == 2) {
								printf("Invalid %s after %s (at token %d)\n", GetName(tokens[no + 1]), GetName(tokens[no]), no + 1);

								return false;
						}
				}

				no++;
		}


		return true;
}

bool CheckNum(Token *tokens) {
		int no = 0;

		while (tokens[no].type != TOKEN_END) {
				if (TOKEN_NUM(tokens[no])) {
						if (no > 1 && tokens[no - 1].type != '(' && ParaNum(tokens[no - 1].type) == 0) {
								printf("Invalid %s befor %s (at token %d)\n", GetName(tokens[no - 1]), GetName(tokens[no]), no + 1);
								return false;
						}

						if (tokens[no + 1].type != TOKEN_END && tokens[no + 1].type != ')' && ParaNum(tokens[no + 1].type) != 2) {
								printf("Invalid %s after %s (at token %d)\n", GetName(tokens[no + 1]), GetName(tokens[no]), no + 1);
								return false;
						}
				}

				++no;
		}

		return true;
}

bool Check(Token *tokens) {
		if (!CheckPare(tokens))
				return false;

		if (!CheckOps(tokens))
				return false;

		if (!CheckNum(tokens))
				return false;

		return true;
}


//check num
