#ifndef _JCC_H_
#define _JCC_H_

#include "cpu/exec/helper.h"

#define FUNC j
#define FUNC_MAKER make_helper

MAKE_CC_HELPER_V()
MAKE_CC_HELPER_B()

#undef FUNC
#undef FUNC_MAKER 

make_helper(jcz);

#endif
