#include "monitor/breakpoint.h"

#include "nemu.h"

#define NR_BP 32
bool break_state;
static BP bp_pool[NR_BP];
static BP *head, *free_;

void init_bp_pool() {
	int i;
	for(i = 0; i < NR_BP - 1; i ++) {
		bp_pool[i].NO = i;
		bp_pool[i].Position=0x100000;
		bp_pool[i].Temp_instr=0x00;
		bp_pool[i].next = &bp_pool[i + 1];
	}
	bp_pool[i].NO=i;
	bp_pool[i].Temp_instr=0x00;
	bp_pool[i].next = NULL;
	bp_pool[i].Position=0x100000;

	head = NULL;
	free_ = bp_pool;
}

/* TODO: Implement the function of breakpoint */
BP* new_bp() {
    if ( head == NULL ) {
			head=free_;
			free_= free_->next;
			head->next= NULL;
			head->NO=1;
			return head;
	}
    if ( free_ != NULL ) {
			BP *new=free_;
			free_=free_->next;
			new->next= NULL;
			BP *p=head;
			while(p->next != NULL){
					p= p->next;
			}
			p->next= new;
			new->NO=p->NO+1;
			return new;
			}
     printf("too many breakpoints!please free some first.\n");
	 return NULL;
}//create a new break

void free_bp(BP* bp) {
    BP* p=head;
	while(p->next != bp){
			p= p->next;
	}
	p->next=bp->next;
	bp->next=NULL;
	BP *new=free_;
	while(new->next != NULL){
			new=new->next;
			new->next=bp;
	}
}//free a break

BP* search_bp(int num) {
		int i=0;
		while(bp_pool[i].NO != num&& i<32) {
				++i;
				}
		if (i == 32) {
				return NULL;
				}
		else return &bp_pool[i];
}

void print_bp() {
    BP* p = head;

	while (p != NULL) {
			printf("Breakpoint %d at eip:%08x\n", p->NO, bp_pool->Position);

			p = p->next;
	}
}// print breakpoints

unsigned char find_bp(swaddr_t eip, bool* success) {
		*success = true;
		int i = 0;
		while (bp_pool[i].Position != eip && eip - bp_pool[i].Position != 0xc0000000 && i < 32) {
				++i;
		}
		if (i == 32) {
				*success = false;
				return 0;
		}
		return bp_pool[i].Temp_instr;
}// restore breakpoint

void load_bp() {
		if (head != NULL) {
				BP *bp = head;
				while (bp != NULL) {
						extern int current_sreg;

						current_sreg = R_CS;
						swaddr_write(bp->Position, 1, 0xcc);
						bp = bp -> next;
				}
		}
}//load bp
