#include "hal.h"

#define NR_KEYS 18
#define CODE_ADDR 0x60

enum {KEY_STATE_EMPTY, KEY_STATE_WAIT_RELEASE, KEY_STATE_RELEASE, KEY_STATE_PRESS};

/* Only the following keys are used in NEMU-PAL. */
static const int keycode_array[] = {
	K_UP, K_DOWN, K_LEFT, K_RIGHT, K_ESCAPE,
	K_RETURN, K_SPACE, K_PAGEUP, K_PAGEDOWN, K_r,
	K_a, K_d, K_e, K_w, K_q,
	K_s, K_f, K_p
};

static int key_state[NR_KEYS];

void
keyboard_event(void) {
	/* TODO: Fetch the scancode and update the key states. */
	//assert(0);

	char code = in_byte(CODE_ADDR);
	char if_rel = code & 0x80;
	code &= 0x7f;
	//int i = 0;

/*	
	for (; i < NR_KEYS; ++i) {
			if (code == keycode_array[i]) {
					if (if_rel)
							key_state[i] = KEY_STATE_RELEASE;
				    else if (key_state[i] == KEY_STATE_EMPTY)
							key_state[i] = KEY_STATE_PRESS;
				    
					break;
			}
	}
*/

#define MAKE_EVENT(i) \
if (code == keycode_array[i]) {\
		if (if_rel) \
				key_state[i] = KEY_STATE_RELEASE; \
		else if (key_state[i] == KEY_STATE_EMPTY)\
				key_state[i] = KEY_STATE_PRESS;\
		return;\
}

#define MAKE_EVENT3(i) \
MAKE_EVENT(i)\
MAKE_EVENT(i + 1)\
MAKE_EVENT(i + 2)

#define MAKE_EVENT9(i) \
MAKE_EVENT3(i)\
MAKE_EVENT3(i + 3)\
MAKE_EVENT3(i + 6)

MAKE_EVENT9(0)
MAKE_EVENT9(9)

}

static inline int
get_keycode(int index) {
	assert(index >= 0 && index < NR_KEYS);
	return keycode_array[index];
}

static inline int
query_key(int index) {
	assert(index >= 0 && index < NR_KEYS);
	return key_state[index];
}

static inline void
release_key(int index) {
	assert(index >= 0 && index < NR_KEYS);
	key_state[index] = KEY_STATE_WAIT_RELEASE;
}

static inline void
clear_key(int index) {
	assert(index >= 0 && index < NR_KEYS);
	key_state[index] = KEY_STATE_EMPTY;
}

bool 
process_keys(void (*key_press_callback)(int), void (*key_release_callback)(int)) {
	cli();
	/* TODO: Traverse the key states. Find a key just pressed or released.
	 * If a pressed key is found, call ``key_press_callback'' with the keycode.
	 * If a released key is found, call ``key_release_callback'' with the keycode.
	 * If any such key is found, the function return true.
	 * If no such key is found, the function return false.
	 * Remember to enable interrupts before returning from the function.
	 */

	//assert(0);
/*
	int i = 0;
	for (; i < NR_KEYS; ++i) {
			if (query_key(i) == KEY_STATE_PRESS) {
					key_press_callback(get_keycode(i));
					release_key(i);

					return true;
			}
			else if (query_key(i) == KEY_STATE_RELEASE) {
					key_release_callback(get_keycode(i));
					clear_key(i);

					return true;
			}
	}
*/

#define MAKE_PROCESS(i) \
if (query_key(i) == KEY_STATE_PRESS) {\
		key_press_callback(get_keycode(i));\
		release_key(i);\
\
		return true;\
}\
else if (query_key(i) == KEY_STATE_RELEASE) {\
		key_release_callback(get_keycode(i));\
		clear_key(i);\
\
		return true;\
}

#define MAKE_PRO3(i)\
MAKE_PROCESS(i)\
MAKE_PROCESS(i + 1)\
MAKE_PROCESS(i + 2)


#define MAKE_PRO9(i)\
MAKE_PRO3(i)\
MAKE_PRO3(i + 3)\
MAKE_PRO3(i + 6)

MAKE_PRO9(0)
MAKE_PRO9(9)

	sti();
	return false;
}
