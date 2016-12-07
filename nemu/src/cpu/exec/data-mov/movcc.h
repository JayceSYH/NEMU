#ifndef __MOVCC_H__
#define __MOVCC_H__

#include "cpu/exec/helper.h"

#define FUNC_MAKER make_helper
#define FUNC mov

MAKE_CC_HELPER()

#undef FUNC
#undef FUNC_MAKER

#endif
