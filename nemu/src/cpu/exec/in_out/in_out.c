#define DATA_BYTE 1
#include "in-template.h"
#include "out-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "in-template.h"
#include "out-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "in-template.h"
#include "out-template.h"
#undef DATA_BYTE

make_helper_v(in_i2a);
make_helper_v(in_d2a);
make_helper_v(out_a2i);
make_helper_v(out_a2d);
