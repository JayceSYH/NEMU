#include "expr.h"
#include <stdlib.h>
#include <stdio.h>

#define CLEAR 0x0

void pop_buff(Token *buff, int *buff_top, Token *output, int *out_top, int priv) {
		while (*buff_top > 0 && buff[(*buff_top) - 1].type != '(' && buff[(*buff_top) - 1].priv >= priv) {
				output[*out_top] = buff[(*buff_top) - 1];
				(*out_top)++;
				(*buff_top)--;
		}

		if (buff[(*buff_top) - 1].type == '(' && priv == CLEAR)
				(*buff_top)--;
}

void Infix2Suffix(Token** _tokens) {
		Token *tokens = *_tokens;
		Token buff[100];
		int buff_top = 0;

		Token *output;
		int out_top = 0;

		int no = 0;
		int count = 0;

		while (tokens[no].type != TOKEN_END) {
				if (tokens[no].type != '(' && tokens[no].type != ')') 
						count++;
				no++;
		}//count tokens

		output = (Token*)malloc(sizeof(Token) * (count + 1));
		output[count].type = TOKEN_END;

		int i = 0;
		for (; i < no; ++i) {
				if (TOKEN_NUM(tokens[i])) {
						output[out_top] = tokens[i];
						++out_top;
						continue;
				}

				if (tokens[i].type == '(') {
						buff[buff_top] = tokens[i];
						++buff_top;
						continue;
				}

				if (tokens[i].type == ')') {
						pop_buff(buff, &buff_top, output, &out_top, CLEAR);
						continue;
				}

				if (buff_top == 0 || tokens[i].priv > buff[buff_top - 1].priv || (ParaNum(tokens[i].type) == 1 && tokens[i].priv == buff[buff_top - 1].priv)) {
						buff[buff_top] = tokens[i];
						++buff_top;
						continue;
				}
				else {
						pop_buff(buff, &buff_top, output, &out_top, tokens[i].priv);
						buff[buff_top] = tokens[i];
						++buff_top;
						continue;
				}
		}//infix 2 suffix

		pop_buff(buff, &buff_top, output, &out_top, CLEAR);
		
#if _DEBUG_ == ON
		assert(out_top == count);
#endif

		free(tokens);

		*_tokens = output;
}


