#ifndef _EXEC_H_
#define _EXEC_H_

#include "common.h"

#define TABLEN 512

#define ON 1
#define OFF 0

#define _DEBUG_ OFF ////////////DEBUG

#include <assert.h>

enum {
	TOKEN_END, NOTYPE , DNUM, REG, TOKEN, HNUM, RS, LS, EQ, G, L, GE, LE, NE, AND, OR, POS, NEG, DP

	/* TODO: Add more token types */

};

typedef struct {
		int type;
		char exp[32];
		unsigned value;
		char priv;
} Token;

typedef struct {
		char *name;
		union {
				unsigned value;
				struct {
					unsigned min_addr;
				    unsigned max_addr;
				};
		};
} VarEntry;


struct rule {
		char *regex;
		int token_type;
};

typedef unsigned (*FP1) (unsigned*);
typedef unsigned (*FP2) (unsigned*, unsigned*);

extern VarEntry VarTab[];
#define _VAR_TAB_ VarTab
#define _TAB_LIMIT_ ((void*)(VarTab + TABLEN))

#define SKIPSPACE(str) \
		while (*str == ' ') ++str;

#define CLEARTAB(tab) \
		int COUNTER_TAB = 0;\
		for (; COUNTER_TAB < TABLEN; ++ COUNTER_TAB) \
				tab[COUNTER_TAB].name = NULL; 

#define TOKEN_NUM(token) \
		(((token).type) <= HNUM && (token).type >= DNUM)

#define VALID_ENTRY(ve) \
		((ve).name)

#define SETEND(token) \
		token.type = END;\

#define _IFNUM_(x) \
		((x) >= '0' && (x) <= '9')

#define IS_NUM(type) (((type) == DNUM) || ((type) == HNUM))

static inline VarEntry make_entry_value(char *str, unsigned value) {
		VarEntry ve;
		ve.name = (char*)malloc(strlen(str) + 1);
		strcpy(ve.name, str);
		ve.value = value;

		return ve;
}

static inline VarEntry make_entry_func(char *str, unsigned max_addr, unsigned min_addr) {
		VarEntry ve;
		ve.name = (char*)malloc(strlen(str) + 1);
		strcpy(ve.name, str);
		ve.max_addr = max_addr;
		ve.min_addr = min_addr;

		return ve;
}
void InitCalcu();
void Infix2Suffix(Token**);
bool Check(Token*);
int ParaNum(char);
unsigned calcu(Token*);
VarEntry* SearchEntry(char *);
char *SearchFunc(unsigned);
void AddEntry(VarEntry);
void SetPos_Neg(Token*, int);
void SetDP(Token*, int);
void SetPriv(Token*, int);
unsigned GetNum(Token *, bool*);

#define SPO single purpose operator
#define MPO multiple prupose operator

#define str_temp(x) #x
#define str(x) str_temp(x)

#endif
