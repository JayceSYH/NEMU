#include "monitor/watchpoint.h"
#include "monitor/expr.h"

#ifndef str
#define str_temp(x) #x
#define str(x) str_temp(x)
#endif

#define WE str(Watch Point Error:)

#ifndef ON
#define ON 1
#define OFF 0
#endif

#define NR_WP 32
#define NO_START 1

static WP wp_list[NR_WP] = {{ .state = OFF }};
static WP *head, *free_;
static int current_no = NO_START;

void init_wp_list() {
	int i;
	for(i = 0; i < NR_WP; i ++) {
		wp_list[i].NO = i;
		wp_list[i].next = &wp_list[i + 1];
	}
	wp_list[NR_WP - 1].next = NULL;

	head = NULL;
	free_ = wp_list;
}

/* TODO: Implement the functionality of watchpoint */
int add_wp(char *str) {
		if (free_ == NULL) {
				printf(WE "too many watch points!\n");
				return -1;
		}
		else if (strlen(str) >= 40) {
				printf(WE "expression is too long\n");
				return -1;
		}

		bool success;
		uint32_t value = expr(str, &success);

		if (!success) {
				printf(WE "invalid expression\n");
				return -1;
		}

		if (head == NULL) {
				head = free_;
				free_ = free_->next;
				head->next = NULL;
				head->state = ON;
				head->saved_value = value;
				head->NO = current_no++;
				strcpy(head->exp, str);
		}
		else {
				WP *p = head;

				while (p->next != NULL) {
						p = p->next;
				}

				p->next = free_;
				p = p->next;
				free_ = free_->next;
				p->next = NULL;
				p->state = ON;
				p->saved_value = value;
				p->NO = current_no++;
				strcpy(p->exp, str);
		}

		return 0;
}

void delete_wp(int no) {
		if (head == NULL) {
				printf(WE "no watch point is set\n");
				return ;
		}
		
		WP *p = head;

		if (head->NO == no) {
				head = head->next;
				p->state = OFF;
				p->next = free_;
				free_ = p;
		}
		else {

		    while (1) {
				if (p->next == NULL) {
						printf(WE "invalid watch point %d\n", no);
						return;
				}

				if (p->next->NO == no) 
						break;

				p = p->next;
		    }
		}

		WP *q = p->next;
		p->next = q->next;
		q->state = OFF;
		q->next = free_;
		free_ = q;
}

int query_wp(swaddr_t eip) {

		int ret = 0;
		WP *p = head;

		while (p != NULL) {
				bool success;
				uint32_t value = expr(p->exp, &success);
				if (!success) {
						printf(WE "Failed to get value: %s\n", p->exp);
						++ret;
				}
				else if (value != p->saved_value) {
						printf("watch stop at %08x: %s\n", eip, p->exp);
						p->saved_value = value;
						++ret;
				}

				p = p->next;
		}


		return ret;
}


void print_wp() {
		if (head == NULL) {
				printf("No Watch Point\n");
				return;
		}

		WP *p = head;

		printf("watch points:\n");

		while (p != NULL) {
				printf("#%d value(%08x) :%s\n", p->NO, p->saved_value, p->exp);

				p = p->next;
		}

}
