#include "nemu.h"
#include "calcu.h"

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>
#include <stdlib.h>

#define TOKEN_SIZE 32

struct rule rules[] = {

	/* TODO: Add more rules.
	 * Pay attention to the precedence level of different rules.
	 */

	{" +",	NOTYPE},				//white space
	{"0[xX][0-9a-zA-Z]{1,9}", HNUM},  //number r16
	{"[0-9]{1,11}", DNUM},             //number
	{"\\$[e]{0,1}[abcdis][xiplh]", REG}, //register
	{"[a-zA-Z_]+", TOKEN},    //token_name
	{"\\(", '('},                     //LP
	{"\\)", ')'},                     //RP
	{"\\*", '*'},                     //mul or dis
	{"\\%", '%'},                    //mod
	{"<<", LS},                      //right shift
	{">>", RS},                     //left shift
	{"/", '/'},                     //div
	{"\\+", '+'},  					//plus
	{"-", '-'},                     //sub
	{"\\|\\|",OR},                  //or
	{"\\|",'|'},                    //ors
	{"\\&\\&",AND},                 //and
	{"\\&",'&'},                    //ands
	{"\\^",'^'},                    //xor
	{"!=", NE},                     //not equel
	{"==", EQ},						//equal
	{">", G},                       //more
	{"<", L},                       //less
	{">=", GE},                     //more or equal
	{"<=", LE},                     //less or equal
	{"!", '!'},                     //not
	{"~", '~'}                      //nots
};

#define NR_REGEX (sizeof(rules) / sizeof(rules[0]) )

static regex_t re[NR_REGEX];
Token *tokens;

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
	int i;
	char error_msg[128];
	int ret;

	for(i = 0; i < NR_REGEX; i ++) {
		ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
		if(ret != 0) {
			regerror(ret, &re[i], error_msg, 128);
			Assert(ret == 0, "regex compilation failed: %s\n%s", error_msg, rules[i].regex);
		}
	}
}


int nr_token;

static bool make_token(char *e) {
	int position = 0;
	int i;
	regmatch_t pmatch;
	
	nr_token = 0;
    if (tokens != NULL) {
		free(tokens);
    }

				tokens = (Token*)malloc(sizeof(Token) * (TOKEN_SIZE + 1));

	while(e[position] != '\0') {
		/* Try all rules one by one. */
		for(i = 0; i < NR_REGEX; i ++) {
			if(regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
				//char *substr_start = e + position;
				int substr_len = pmatch.rm_eo;

				//Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s", i, rules[i].regex, position, substr_len, substr_len, substr_start);
				position += substr_len;

				/* TODO: Now a new token is recognized with rules[i]. Add codes
				 * to record the token in the array ``tokens''. For certain 
				 * types of tokens, some extra actions should be performed.
				 */
				assert(nr_token < TOKEN_SIZE);


				switch(rules[i].token_type) {
				    case NOTYPE: break;//NO TYPE

				    case REG:
					case TOKEN:
				    case DNUM:
					case HNUM:{
				    bool success;
					tokens[nr_token].type = rules[i].token_type; strncpy(tokens[nr_token].exp,e+position-substr_len,substr_len);tokens[nr_token].exp[substr_len] = 0;
                    tokens[nr_token].value = GetNum(&tokens[nr_token], &success); 
					++nr_token; 
					if (!success) 
							return false;
					}break;//NUM

					case '(': case ')':
					case '+': case '-':
					case '*': case '/':
					case '%': case AND:
					case OR:  case '&':
					case '|': case '^':
					case '!': case '~':
					case EQ:  case G:
					case L:   case GE:
					case LE:  case NE:
					case LS:  case RS:
					tokens[nr_token].type = rules[i].token_type; 
					strncpy(tokens[nr_token].exp,e+position-substr_len,substr_len);tokens[nr_token].exp[substr_len] = 0;
					++nr_token; break;//CALCU TOKEN

					default: panic("please implement me");
				}

				break;
			}
		}

		if(i == NR_REGEX) {
			printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
			return false;
		}
	}
		
    tokens[nr_token++].type = TOKEN_END;

	return true; 
}

uint32_t expr(char *e, bool *success) {
	if(!make_token(e)) {
		*success = false;
		return 0;
	}


	/* TODO: Insert codes to evaluate the expression. */

    *success = true;
	//check
	SetPos_Neg(tokens, nr_token);
	SetDP(tokens, nr_token);
	SetPriv(tokens, nr_token);

	if (!Check(tokens)) {
			*success = false;
			return 0;
	}

    Infix2Suffix((Token**)&tokens);

	return calcu(tokens);
}

