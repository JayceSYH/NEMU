#include "expr.h"
#include <stdlib.h>
#include <string.h>

VarEntry VarTab[TABLEN];

void InitCalcu() {
		CLEARTAB(_VAR_TAB_);
}

VarEntry *SearchEntry(char *name) {
		VarEntry *tab = _VAR_TAB_;
		while (tab->name != NULL) {
				if (!strcmp(tab->name, name))
						return tab;
				++tab;
		}

		return NULL;
}

char *SearchFunc(unsigned addr) {
		VarEntry *tab = _VAR_TAB_;
		while (tab->name != NULL) {
				if (tab->max_addr >= addr && tab->min_addr <= addr) {
						return tab->name;
				}

				++tab;
		}

		return NULL;
}

void AddEntry(VarEntry ve) {
		VarEntry *pOld = SearchEntry(ve.name);
		if (pOld) {
				*pOld = ve;
				return ;
		}
		else {
				VarEntry *tab = _VAR_TAB_;
				while (tab->name != NULL) {
						++tab;
				}

				assert((unsigned)tab < (unsigned)_TAB_LIMIT_);

				*tab = ve;
		}
}
