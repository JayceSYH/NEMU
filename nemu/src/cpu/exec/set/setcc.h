#ifndef _SETCC_H
#define _SETCC_H

#include "cpu/exec/helper.h"

#define FUNC set
#define FUNC_MAKER make_helper

MAKE_CC_HELPER()

#undef FUNC
#undef FUNC_MAKER

#endif
