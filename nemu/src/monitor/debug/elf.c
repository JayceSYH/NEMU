#include "memory/memory.h"

#include "common.h"
#include "calcu/expr/expr.h"
#include <stdlib.h>
#include <elf.h>

void get_func();

char *exec_file = NULL;

static int str_size;
static char *strtab = NULL;
static Elf32_Sym *symtab = NULL;
static int nr_symtab_entry;

void load_elf_tables(int argc, char *argv[]) {
	int ret;
	Assert(argc == 2, "run NEMU with format 'nemu [program]'");
	exec_file = argv[1];

	FILE *fp = fopen(exec_file, "rb");
	Assert(fp, "Can not open '%s'", exec_file);

	uint8_t buf[4096];
	/* Read the first 4096 bytes from the exec_file.
	 * They should contain the ELF header and program headers. */
	ret = fread(buf, 4096, 1, fp);
	//assert(ret == 1);

	/* The first several bytes contain the ELF header. */
	Elf32_Ehdr *elf = (void *)buf;
	char magic[] = {ELFMAG0, ELFMAG1, ELFMAG2, ELFMAG3};

	/* Check ELF header */
	assert(memcmp(elf->e_ident, magic, 4) == 0);		// magic number
	assert(elf->e_ident[EI_CLASS] == ELFCLASS32);		// 32-bit architecture
	assert(elf->e_ident[EI_DATA] == ELFDATA2LSB);		// littel-endian
	assert(elf->e_ident[EI_VERSION] == EV_CURRENT);		// current version
	assert(elf->e_ident[EI_OSABI] == ELFOSABI_SYSV || 	// UNIX System V ABI
			elf->e_ident[EI_OSABI] == ELFOSABI_LINUX); 	// UNIX - GNU
	assert(elf->e_ident[EI_ABIVERSION] == 0);			// should be 0
	assert(elf->e_type == ET_EXEC);						// executable file
	assert(elf->e_machine == EM_386);					// Intel 80386 architecture
	assert(elf->e_version == EV_CURRENT);				// current version


	/* Load symbol table and string table for future use */

	/* Load section header table */
	uint32_t sh_size = elf->e_shentsize * elf->e_shnum;
	Elf32_Shdr *sh = malloc(sh_size);
	fseek(fp, elf->e_shoff, SEEK_SET);
	ret = fread(sh, sh_size, 1, fp);
	assert(ret == 1);

	/* Load section header string table */
	char *shstrtab = malloc(sh[elf->e_shstrndx].sh_size);
	fseek(fp, sh[elf->e_shstrndx].sh_offset, SEEK_SET);
	ret = fread(shstrtab, sh[elf->e_shstrndx].sh_size, 1, fp);
	assert(ret == 1);

	int i;
	for(i = 0; i < elf->e_shnum; i ++) {
		if(sh[i].sh_type == SHT_SYMTAB && 
				strcmp(shstrtab + sh[i].sh_name, ".symtab") == 0) {
			/* Load symbol table from exec_file */
			symtab = malloc(sh[i].sh_size);
			fseek(fp, sh[i].sh_offset, SEEK_SET);
			ret = fread(symtab, sh[i].sh_size, 1, fp);
			assert(ret == 1);
			nr_symtab_entry = sh[i].sh_size / sizeof(symtab[0]);
		}
		else if(sh[i].sh_type == SHT_STRTAB && 
				strcmp(shstrtab + sh[i].sh_name, ".strtab") == 0) {
			/* Load string table from exec_file */
            str_size = sh[i].sh_size;
			strtab = malloc(sh[i].sh_size);
			fseek(fp, sh[i].sh_offset, SEEK_SET);
			ret = fread(strtab, sh[i].sh_size, 1, fp);
			assert(ret == 1);
		}
	}

	free(sh);
	free(shstrtab);

	assert(strtab != NULL && symtab != NULL);

	fclose(fp);

	get_func();
}



swaddr_t find_sym(char *sym_name, bool *success) {
		char fc = *sym_name;
		char *pt = strtab + 1;
		while(pt != strtab + str_size) {
				if (*pt == fc) {
						if (!strcmp(pt, sym_name)) {
								int offset = pt - strtab;
								int i = 0;
								for (; i < nr_symtab_entry; ++i) {
										if (symtab[i].st_name == offset) {
												*success = true;
												return symtab[i].st_value;
										}
								}
								assert(0);//not find the entry??
						}
				}
				++pt;
		}
		*success = false;
		fprintf(stderr, "error:invalid expression:%s", sym_name);
		return 0;
}//find symbol according to name

void get_func() {
		int i = 0;
		for (; i < nr_symtab_entry; ++i) {
				if (ELF32_ST_TYPE(symtab[i].st_info) != STT_FUNC) {
						continue;
				}
				else {
						unsigned min_addr, max_addr;
						min_addr = symtab[i].st_value;
						max_addr = min_addr + symtab[i].st_size - 1;

						AddEntry(make_entry_func(strtab + symtab[i].st_name, max_addr, min_addr));
				}
		}
}

char* get_funcname(swaddr_t addr) {
		return SearchFunc(addr);
}

