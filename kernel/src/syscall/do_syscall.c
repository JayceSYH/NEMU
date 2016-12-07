#include "irq.h"

#include <sys/syscall.h>

void add_irq_handle(int, void (*)(void));
void mm_brk(uint32_t);

static void sys_brk(TrapFrame *tf) {
#ifdef IA32_PAGE
	mm_brk(tf->ebx);
#endif
	tf->eax = 0;
}

int fs_open(char*);
int fs_read(int , void*, int);
int fs_write(int , void*, int);
int fs_lseek(int, int, int);
int fs_close(int);

void do_syscall(TrapFrame *tf) {
	switch(tf->eax) {
		/* The ``add_irq_handle'' system call is artificial. We use it to 
		 * let user program register its interrupt handlers. But this is 
		 * very dangerous in a real operating system. Therefore such a 
		 * system call never exists in GNU/Linux.
		 */
		case 0: 
			cli();
			add_irq_handle(tf->ebx, (void*)tf->ecx);
			sti();
			break;

		case SYS_brk: sys_brk(tf); break;

		/* TODO: Add more system calls. */
		case SYS_write: {
				int len = tf->edx;
				int fd_no = tf->ebx;
				void *buf = (void*)tf->ecx;
				tf->eax = fs_write(fd_no, buf, len);

				break;
		}

		case SYS_read: {
				int len = tf->edx;
				int fd_no = tf->ebx;
				void *buf = (void*)tf->ecx;

				tf->eax = fs_read(fd_no, buf, len);

				break;
		}

		case SYS_open:{
				char *name = (char*)tf->ebx;

				tf->eax = fs_open(name);

				break;
		}

		case SYS_lseek: {
				int fd_no = tf->ebx;
				int offset = tf->ecx;
				int whence = tf->edx;

				tf->eax = fs_lseek(fd_no, offset, whence);

				break;
		}

		case SYS_close: {
				int fd_no = tf->ebx;

				tf->eax = fs_close(fd_no);

				break;
		}

		default: panic("Unhandled system call: id = %d", tf->eax);
	}
}

