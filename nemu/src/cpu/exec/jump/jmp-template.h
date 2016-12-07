#include "cpu/exec/template-start.h"

#define JMP_REL(c) (c == 0xEB || c == 0xE9)
#define JMP_NEAR(c) (c == 0xFF)
#define JMP_INTERP(c) (c == 0xEA)

#define instr jmp

static void do_execute() {
		uint8_t opcode = instr_fetch(cpu.eip, 1);
        
        if (JMP_REL(opcode)) {
                    cpu.eip += (int32_t)(DATA_TYPE_S)op_src->val;
                    print_asm("jmp %x", cpu.eip + 1 + DATA_BYTE);
     }
     else if (JMP_NEAR(opcode)) {
                        if (op_src->type == OP_TYPE_REG) {
                            cpu.eip = op_src->val - 2;
                            print_asm("jmp *%s", op_src->str);
                        }
                        else {
                            Operand temp_rm, temp_reg;
							temp_rm.size = DATA_BYTE;
							temp_reg.size = DATA_BYTE;
							extern int read_ModR_M(swaddr_t, Operand*, Operand*);
                            int ins_len = read_ModR_M(cpu.eip + 1, &temp_rm, &temp_reg);
                            cpu.eip = (DATA_TYPE)op_src->val - 1 - ins_len;
                            print_asm("jmp *%s", op_src->str);
                        }
    }
	else { assert(0); }
         
}

make_instr_helper(i);
make_instr_helper(rm);

make_helper(concat(jmp_interp_, SUFFIX)) {
        sreg(R_CS) = instr_fetch(cpu.eip + 1 + DATA_BYTE, 2);
        sreg_update(R_CS);

		swaddr_t destination = 0;

#if DATA_BYTE == 2
		destination = instr_fetch(cpu.eip + 1, 2);
		cpu.eip = (destination & 0xffff) - 5;
#else
		destination = instr_fetch(cpu.eip + 1, 4);
		cpu.eip = destination - 7;
#endif

		print_asm("ljmp $0x%x,$0x%x", sreg(R_CS), destination);
		
		return 1 + 2 + DATA_BYTE;
}

#include "cpu/exec/template-end.h"
