void init_monitor(int, char *[]);
void reg_test();
void restart();
void ui_mainloop();
void tlb_init();
void init_device();
void init_sdl();
void sdl_clear_event_queue();
void cache_reset();
#include "cpu/reg.h"
#include "common.h"

int main(int argc, char *argv[]) {

	/* Initialize the monitor. */
	init_monitor(argc, argv);

	/* Initialize the cache */
	cache_reset();

	/* Test the implementation of the ``CPU_state'' structure. */
	reg_test();
	tlb_init();
	cr0.val = 0;

	/* Initialize the virtual computer system. */
	restart();

	/* Initialize deivces*/
#ifdef HAS_DEVICE
	init_device();
	init_sdl();
#endif

	/* Receive commands from user. */
	ui_mainloop();

	return 0;
}
