#include "cpu/exec/helper.h"

#include "popa.h"
#include "popad.h"
#include "pusha.h"
#include "pushad.h"

#define pushx_l pushad
#define pushx_w pusha
#define popx_l popad
#define popx_w popa

make_helper_v(pushx);
make_helper_v(popx);
