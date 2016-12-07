#define DATA_BYTE 2
#include "movcc-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "movcc-template.h"
#undef DATA_BYTE

#define _MOVCC_(cc) \
make_helper_v(concat(mov,cc));\
inline make_helper(concat(mov, cc)) {\
		return concat4(mov, cc, _, v)(eip);\
}

_MOVCC_(a)
_MOVCC_(ae)
_MOVCC_(b)
_MOVCC_(be)
_MOVCC_(e)
_MOVCC_(g)
_MOVCC_(ge)
_MOVCC_(l)
_MOVCC_(le)
_MOVCC_(ne)
_MOVCC_(no)
_MOVCC_(np)
_MOVCC_(ns)
_MOVCC_(o)
_MOVCC_(p)
_MOVCC_(s)
