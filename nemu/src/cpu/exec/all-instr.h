#include "prefix/prefix.h"

#include "data-mov/mov.h"
#include "data-mov/xchg.h"
#include "data-mov/mov_s.h"
#include "data-mov/movxx.h"
#include "data-mov/movs.h"
#include "data-mov/movcc.h"
#include "data-mov/cxx.h"

#include "arith/dec.h"
#include "arith/inc.h"
#include "arith/neg.h"
#include "arith/imul.h"
#include "arith/mul.h"
#include "arith/idiv.h"
#include "arith/div.h"
#include "arith/add.h"
#include "arith/adc.h"
#include "arith/sub.h"
#include "arith/sbb.h"
#include "arith/cmp.h"
#include "arith/cmps.h"

#include "logic/and.h"
#include "logic/or.h"
#include "logic/not.h"
#include "logic/xor.h"
#include "logic/sar.h"
#include "logic/shl.h"
#include "logic/shr.h"
#include "logic/shrd.h"
#include "logic/shld.h"
#include "logic/test.h"
#include "logic/bt.h"

#include "jump/jmp.h"
#include "jump/call.h"
#include "jump/jcc.h"

#include "push_pop/push_pop.h"

#include "set/set.h"
#include "set/clear.h"
#include "set/setcc.h"

#include "ret/ret.h"
#include "ret/leave.h"

#include "lea/lea.h"

#include "string/rep.h"
#include "string/stos.h"

#include "misc/misc.h"
#include "int/int.h"

#include "load/load.h"

#include "in_out/in_out.h"

#include "special/special.h"
