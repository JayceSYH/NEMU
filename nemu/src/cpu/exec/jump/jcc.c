#include "cpu/exec/helper.h"

#define DATA_BYTE 1
#include "jcc-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "jcc-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "jcc-template.h"
#undef DATA_BYTE

#define FUNC j
#define FUNC_MAKER make_helper_v

MAKE_CC_HELPER()

#undef FUNC
#undef FUNC_MAKER

make_helper(jcz) {
		if (IS16) 
				return jcxz_b(eip);
		else 
				return jecxz_b(eip);
}

