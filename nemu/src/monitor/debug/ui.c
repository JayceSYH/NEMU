#include "monitor/monitor.h"
#include "monitor/expr.h"
#include "monitor/watchpoint.h"
#include "monitor/breakpoint.h"
#include "nemu.h"
#include "cpu/reg.h"

#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

#define INT3_CODE 0xcc

void cpu_exec(uint32_t);
void restart();

/* We use the ``readline'' library to provide more flexibility to read from stdin. */
char* rl_gets() {
	static char *line_read = NULL;

	if (line_read) {
		free(line_read);
		line_read = NULL;
	}

	line_read = readline("(nemu) ");

	if (line_read && *line_read) {
		add_history(line_read);
	}

	return line_read;
}

static int cmd_c(char *args) {
	cpu_exec(-1);
	return 0;
}

static int cmd_q(char *args) {
	return -1;
}

static int cmd_help(char *args);
static int cmd_si(char *args);
static int cmd_x(char *args);
static int cmd_info(char *args);
static int cmd_bt(char *args);
static int cmd_w(char *args);
static int cmd_wd(char *args);
static int cmd_d(char *args);
static int cmd_b(char *args);
static int cmd_p(char *args);

static struct {
	char *name;
	char *description;
	int (*handler) (char *);
} cmd_table [] = {
    {"p/x", "Print the value of expression(H)", cmd_p },
    {"p", "Print the value of expression(D)", cmd_p },
	{ "help", "Display informations about all supported commands", cmd_help },
	{ "c", "Continue the execution of the program", cmd_c },
	{ "q", "Exit NEMU", cmd_q },
	{ "si", "Single Step", cmd_si },
	{ "x", "Show Stack", cmd_x },
	{ "info", "Informations", cmd_info },
	{ "bt", "Print Stack Frame List", cmd_bt },
	{ "w", "Set Watch Point", cmd_w },
	{ "b", "Set Break Point", cmd_b },
	{ "bd", "Delete Break Point", cmd_d },
	{ "d", "Delete Watch Point", cmd_wd }

	/* TODO: Add more commands */

};

#define P_POS 0

#define NR_CMD (sizeof(cmd_table) / sizeof(cmd_table[0]))

static int cmd_help(char *args) {
	/* extract the first argument */
	char *arg = strtok(NULL, " ");
	int i;

	if(arg == NULL) {
		/* no argument given */
		for(i = 0; i < NR_CMD; i ++) {
			printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
		}
	}
	else {
		for(i = 0; i < NR_CMD; i ++) {
			if(strcmp(arg, cmd_table[i].name) == 0) {
				printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
				return 0;
			}
		}
		printf("Unknown command '%s'\n", arg);
	}
	return 0;
}

static int cmd_si(char *args) {
	    if (nemu_state == END) {
				restart();
				char c;
				while(1) {
						printf("The program is not running. Start the program? (y or n)");
						fflush(stdout);
						scanf(" %c", &c);
						switch (c) {
								case 'y': nemu_state = RUNNING;goto _start;
								case 'n':return 0;
								default: puts("Please answer y or n.");fflush(stdout);
						}
				}
		}
		else nemu_state = RUNNING;
    _start: {
    char *p=strtok(NULL," ");
    if( p == NULL ) {
		 cpu_exec(1);
	     }
    else {
			cpu_exec(atoi(p));
		 }
		 nemu_state= nemu_state == END?END:STOP;
	}

	return 0;
}

static int  cmd_info(char *args) {
    char *p=strtok(NULL," ");
	if( p == NULL ) {
		 return 0;
	     }
    else {  if(!strcmp(p,"r")) { print_reg(); }
	        //else if (!strcmp(p,"b")) { print_bp(); }
			else if (!strcmp(p,"e")) { print_eflags; }
			else if (!strcmp(p,"w")) { print_wp(); }
	}

	return 0;
}

static int cmd_p(char *cmd) {

		char *exp=cmd;
		char format = 0;

		while (*exp != 'p') {
				++exp;
		}

		++exp;

		if (*exp == '/') {
				++exp;
				switch (*exp) {
						case 'x':format = 'x';break;
						case 'c':format = 'c';break;
						case 's':format = 's';break;
						default: printf("invalid format '%c'",*exp);return 0;
				}
				++exp;
		}
		else if (*exp != ' ') {
				fprintf(stderr, "invalid expression");
				return 0;
		}
		else format = 'd';

		while(*exp == ' ') {
				++exp;
		}

		if (exp == NULL) return 0;
		bool success;
		int32_t value = expr(exp, &success);
		if (success == false) printf("invalid expression:%s\n",exp);
		else {
				switch (format) {
						case 'd':printf("%d\n", value);break;
						case 'x':printf("0x%x\n", value);break;
						case 'c':printf("%c\n", value);break;
						case 's':{
								char c = swaddr_read(value, 1);
								while(c != 0) {
										printf("%c", c);
										c = swaddr_read(++value, 1);
								}
								printf("\n");
						}break;
				}
		}

		return 0;

}

static int cmd_w(char *cmd) {

		add_wp(cmd);

		return 0;

}

static int cmd_wd(char *args) {

		bool success;
		int no = expr(args, &success);

		if (success)
				delete_wp(no);
		else
				printf("Watch Point Delete Error:invalid expression: %s\n", args);

		return 0;
}

static int cmd_b(char *cmd) {
	   char* exp=cmd;

		while (*exp == ' ') {
				++exp;
		}

		if (*exp == 0) {
				printf("x error:arguments missing\n");
				return 0;
		}
		else {
				int count=0;
				while(*exp != 0) {
						++exp;
						++count;
				}
				char *num=(char*)malloc(count+1);
				num[count]=0;
				strncpy(num,exp-count,count);
				bool success;
				swaddr_t addr;
				addr = expr(num, &success);
				free(num);
				if (success == false) {
						printf("invalid addr\n"); 
						return 0;
				}
				BP *new=new_bp();
				if (new == NULL) return 0;
				new->Position = addr;
				new->Temp_instr= swaddr_read(addr, 1);
				swaddr_write(addr, 1, INT3_CODE);
		}

		return 0;

}

static int cmd_d(char *args) {
		char *p=strtok(NULL," ");
		int num=atoi(p);
		if ( num == 0&&*p != '0'){printf("invalid break point %2d",num);
		BP* bp=search_bp(num);
		if (bp == NULL) {
				return 0;
				}
		else free_bp(bp);
		}

		return 0;
}

static int cmd_x(char* cmd) {
		char* exp=cmd;

		int times = 1;

		while (*exp == ' ') {
				++exp;
		}

		if (*exp == 0) {
				printf("cmd 'x' error :arguments missing\n");
				return 0;
		}


		int len = 0;
		while (*exp != ' ' && *exp != 0) {
				++len;
				++exp;
		}

		char *num = malloc(len + 1);

		num[len] = 0;
		strncpy(num, exp - len, len);
		times = atoi(num);
		free(num);
		times = (times <= 0)? 1:times;

		while (*exp == ' ') {
				++exp;
		}
		if (*exp == 0) {
				printf("cmd 'x' error:arguments missing\n");
				return 0;
		}
		else {
				int count=0;
				while(*exp != 0) {
						++exp;
						++count;
				}
				char *num=(char*)malloc(count+1);
				num[count]=0;
				strncpy(num,exp-count,count);
				bool success;
				swaddr_t addr;
				addr = expr(num, &success);
				free(num);

				if (success == false) {
						printf("invalid addr\n"); 
						return 0;
						}
				int i = 0;
				while (i < times) {
						if (i % 4 == 0) 
								printf("0x%08x:  ", addr + i * 4);

						printf(" 0x%08x", swaddr_read(addr + i * 4, 4));

						if ((i + 1) % 4 == 0) 
								printf("\n");
						++i;
				}
				printf("\n");
		}

		return 0;
}

char* get_funcname(swaddr_t);

static int cmd_bt(char *args) {
		swaddr_t eip = cpu.eip;
		swaddr_t ebp = cpu.ebp;
		char * name = get_funcname(eip);
		int i = 0;
		if (name == NULL && !If) {
				printf("IN INTR\n");
				while (name == NULL) {
						eip = swaddr_read(ebp + 4, 4);
						ebp = swaddr_read(ebp, 4);
						name = get_funcname(eip);
				}
		}

		while (strcmp(name, "main") != 0) {
				printf("#%d 0x%08x in %s ()\n", i, eip, name);
				if (strcmp(name, "game_init") == 0)
						break;
				eip = swaddr_read(ebp + 4, 4);
				ebp = swaddr_read(ebp, 4);
				if (ebp >= 0xc0000000 -16) break;
				name = get_funcname(eip);
				++i;
		}
		printf("\n");

		return 0;
}

extern int quiet;

void ui_mainloop() {
	while(1) {
		char *str = rl_gets();
		char *str_end = str + strlen(str);
        char *temp_cmd=(char*)malloc(strlen(str)+1);
		strcpy(temp_cmd,str);

		/* extract the first token as the command */
		char *cmd = strtok(str, " ");
		if(cmd == NULL) { continue; }

		/* treat the remaining string as the arguments,
		 * which may need further parsing
		 */
		char *args = cmd + strlen(cmd) + 1;
		if(args >= str_end) {
			args = NULL;
		}

#ifdef HAS_DEVICE
		extern void sdl_clear_event_queue(void);
		sdl_clear_event_queue();
#endif

		int i;
		for(i = 0; i < NR_CMD; i ++) {
			if(strcmp(cmd, cmd_table[i].name) == 0) {
				if(cmd_table[i].handler((i == P_POS || i == (P_POS + 1))?temp_cmd:args) < 0) { return; }
				break;
			}
		}

		if(i == NR_CMD) { printf("Unknown command '%s'\n", cmd); }
	}
}
