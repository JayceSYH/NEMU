#include "common.h"

#include <string.h>

typedef struct {
	char *name;
	uint32_t size;
	uint32_t disk_offset;
} file_info;

typedef struct {
		bool opened;
		uint32_t offset;
} Fstate;

enum {SEEK_SET, SEEK_CUR, SEEK_END};

/* This is the information about all files in disk. */
static const file_info file_table[] = {
	{"1.rpg", 188864, 1048576}, {"2.rpg", 188864, 1237440},
	{"3.rpg", 188864, 1426304}, {"4.rpg", 188864, 1615168},
	{"5.rpg", 188864, 1804032}, {"abc.mkf", 1022564, 1992896},
	{"ball.mkf", 134704, 3015460}, {"data.mkf", 66418, 3150164},
	{"desc.dat", 16027, 3216582}, {"fbp.mkf", 1128064, 3232609},
	{"fire.mkf", 834728, 4360673}, {"f.mkf", 186966, 5195401},
	{"gop.mkf", 11530322, 5382367}, {"map.mkf", 1496578, 16912689},
	{"mgo.mkf", 1577442, 18409267}, {"m.msg", 188232, 19986709},
	{"mus.mkf", 331284, 20174941}, {"pat.mkf", 8488, 20506225},
	{"rgm.mkf", 453202, 20514713}, {"rng.mkf", 4546074, 20967915},
	{"sss.mkf", 557004, 25513989}, {"voc.mkf", 1997044, 26070993},
	{"wor16.asc", 5374, 28068037}, {"wor16.fon", 82306, 28073411},
	{"word.dat", 5650, 28155717},
};

#define NR_FILES (sizeof(file_table) / sizeof(file_table[0]))

void ide_read(uint8_t *, uint32_t, uint32_t);
void ide_write(uint8_t *, uint32_t, uint32_t);

/* TODO: implement a simplified file system here. */


Fstate fd[NR_FILES + 3];

void init_fs() {
		int i = 0;
		for (; i < NR_FILES + 3; ++i) {
				fd[i].opened = false;
				fd[i].offset = 0;
		}

		fd[1].opened = true;
		fd[2].opened = true;
}

int fs_open(const char *pathname, int flags) {
		int i = 0;
		for (; i < NR_FILES; ++i) 
				if (strcmp(pathname, file_table[i].name) == 0) {
						fd[i + 3].offset = 0;
						fd[i + 3].opened = true;
						return i + 3;
				}

		Log("%s", pathname);
		assert(0);
		return 0;
}
		
int fs_read(int fd_no, void *buf, int len) {
		if (fd_no == 1 || fd_no == 2 || fd_no == 0) {
				assert(0);
		}

		assert(fd[fd_no].opened);
		int size = file_table[fd_no - 3].size;

		int start = fd[fd_no].offset;

		if (size == start)
				return 0;

		len = ((len + start) <= size)?len:(size - start);
		ide_read(buf, file_table[fd_no - 3].disk_offset + start, len);

		fd[fd_no].offset = start + len;

		return len;
}


int fs_write(int fd_no, void *buf, int len) {
		if (fd_no == 1 || fd_no == 2) {
				int i = 0;
				char *p = (char*)buf;
				extern void serial_printc(char);

				for (; i < len; ++i)
						serial_printc(*(p + i));

				return len;
		}

		assert(fd[fd_no].opened);
		int size = file_table[fd_no - 3].size;

		int start = fd[fd_no].offset;

		if (start == size)
				return 0;

		len = (len + start <= size)?len:(size -start);
		ide_write(buf, file_table[fd_no - 3].disk_offset + start, len);

		fd[fd_no].offset = start + len;

		return len;
}

int fs_lseek(int fd_no, int offset, int whence) {
		//Log("seek fd  %d offset %d whence %d\n", fd_no, offset, whence);
		if (fd_no == 1 || fd_no == 2 || fd_no == 0) {
				assert(0);
		}
		assert(fd[fd_no].opened);
		int start = 0;
		switch (whence) {
				case SEEK_SET:start = 0;break;
				case SEEK_CUR:start = fd[fd_no].offset;break;
				case SEEK_END:start = file_table[fd_no - 3].size;break;
				default:assert(0);
		}

		int size = file_table[fd_no - 3].size;

		int temp_offset = start + offset;
		temp_offset = (temp_offset >= 0)?temp_offset:0;

		fd[fd_no].offset = (temp_offset <= size)?temp_offset:(size - 1);

		return fd[fd_no].offset;
}

int fs_close(int fd_no) {
		if (fd_no == 1 || fd_no == 2) {
				assert(0);
		}
		assert(fd[fd_no].opened);

		fd[fd_no].opened =false;

		return 0;
}
