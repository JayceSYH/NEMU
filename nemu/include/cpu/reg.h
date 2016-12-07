#ifndef __REG_H__
#define __REG_H__

#include "common.h"
#include "desc.h"
#include "page.h" 
#include "flag.h"

enum { R_ES, R_CS, R_SS, R_DS };
enum { R_EAX, R_ECX, R_EDX, R_EBX, R_ESP, R_EBP, R_ESI, R_EDI };
enum { R_AX, R_CX, R_DX, R_BX, R_SP, R_BP, R_SI, R_DI };
enum { R_AL, R_CL, R_DL, R_BL, R_AH, R_CH, R_DH, R_BH };

/* TODO: Re-organize the `CPU_state' structure to match the register
 * encoding scheme in i386 instruction format. For example, if we
 * access cpu.gpr[3]._16, we will get the `bx' register; if we access
 * cpu.gpr[1]._8[1], we will get the 'ch' register. Hint: Use `union'.
 * For more details about the register encoding scheme, see i386 manual.
 */

typedef struct {
		SegDesc sbuff;
		uint16_t sreg;
} Sreg;


typedef struct {
		PTE pte;
		uint32_t tag		:20;
		uint32_t valid_bit	:1;
} TLBE;


typedef struct {
    union {
	union _REG{
		uint32_t _32;
		uint16_t _16;
		uint8_t _8[2];
	} gpr[8];

	/* Do NOT change the order of the GPRs' definitions. */

    struct {
	uint32_t eax, ecx, edx, ebx, esp, ebp, esi, edi;
	};
    };

	swaddr_t eip;
    
     union {
		struct{
			uint32_t CF :1;
			uint32_t PF :1;
			uint32_t ZF :1;
			uint32_t SF :1;
			uint32_t IF :1;
			uint32_t DF :1;
			uint32_t OF :1;
			uint32_t AF :1;
			uint32_t :0;
		};
		
		uint32_t val;
	}eflags;
    
    struct {
			uint32_t base;
			uint16_t limit;
	} gdtr;
    
    struct {
			uint32_t base;
			uint16_t limit;
	} idtr;
    
    union {
			struct {//for 0
				uint32_t pe:1;//paging disable
				uint32_t mc:1;//monitor corprocessor
				uint32_t em:1;//emulator
				uint32_t ts:1;//task swtiched
				uint32_t et:1;//extension type
				uint32_t ne:1;//numeric error
				uint32_t pad0:10;//pad0
				uint32_t wp:1;//write_protect
				uint32_t pad1:1;//pad1
				uint32_t am:1;//alignment mask
				uint32_t pd2:10;//pad2
				uint32_t nwt:1;//no write through
				uint32_t cd:1;//cache disable
				uint32_t pg:1;//paging
			};
			uint32_t val;
		   } cr0;
           
           union {
			struct {
					uint32_t pad0				:3;
					uint32_t pwt				:1;//page write through
					uint32_t pcd				:1;//page cache disable
					uint32_t pad1				:7;
					uint32_t pdb				:20;//page directory base
			};
			uint32_t val;
	} cr3;
    
    Sreg sregs[4];

volatile bool INTR;//interrupt
} CPU_state;

extern CPU_state cpu;

static inline int check_reg_index(int index) {
	assert(index >= 0 && index < 8);
	return index;
}

void REG_INIT();


#define reg_l(index) (cpu.gpr[check_reg_index(index)]._32)
#define reg_w(index) (cpu.gpr[check_reg_index(index)]._16)
#define reg_b(index) (cpu.gpr[check_reg_index(index) & 0x3]._8[index >> 2])

extern const char* regsl[];
extern const char* regsw[];
extern const char* regsb[];
extern const char* sregs[];

void sreg_update(int);
void print_reg();

//Other defination
#define gdtr cpu.gdtr
#define idtr cpu.idtr
#define sreg(index) (cpu.sregs[index].sreg)
#define sbuff(index) (cpu.sregs[index].sbuff)
#define seg_index(index) ((sreg(index) >> 3) & 0x1fff)
#define cr0 cpu.cr0
#define cr3 cpu.cr3
#define pd_base ((cr3.pdb) << 12)

//fast switch
#define buff_base(index) (sbuff(index).base_15_0 + (sbuff(index).base_23_16 << 16) + (sbuff(index).base_31_24 << 24))
#define buff_limit(index) (sbuff(index).limit_15_0 + (sbuff(index).limit_19_16 << 16))
#define uint_to_cr0(num) { cr0.val = num; }
#define cr0_to_uint(num) { num = cr0.val; }
#endif
