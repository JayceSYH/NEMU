#include "common.h"
#include "debug.h"

#define line_num_l1 8
#define line_num_l2 16

#define set_num_l1 (((64*1024)/8)/64)
#define set_num_l2 (((4096*1024)/16)/64)

#define _ON 1
#define _OFF 0

#define cache_debug _OFF

#define to_cache_addr_l1(addr,cache_addr) \
		{\
		  cache_addr.tag = (addr & 0xffffe000) >> 13;\
		  cache_addr.index = (addr & 0x1fc0) >> 6;\
		  cache_addr.offset = addr & 0x3f;\
		  }

#define to_cache_addr_l2(addr,cache_addr) \
		{\
		  cache_addr.tag = (addr & 0xfffc0000) >> 18;\
		  cache_addr.index = (addr & 0x3ffc0) >> 6;\
		  cache_addr.offset = addr & 0x3f;\
		  }

#define to_addr_l1(cache_addr) ((((uint32_t)cache_addr.tag) << 13) + (((uint32_t)cache_addr.index) << 6) +cache_addr.offset)

#define to_addr_l2(cache_addr) ((((uint32_t)cache_addr.tag) << 18) + (((uint32_t)cache_addr.index) << 6) +cache_addr.offset)

#define READ2(ptr) (*(ptr) + (((uint32_t)*(ptr + 1)) << 8))
#define READ3(ptr) (READ2(ptr) + (((uint32_t)*(ptr + 2)) << 16))
#define READ4(ptr) (READ3(ptr) + (((uint32_t)*(ptr + 3)) << 24))

#define WRITE2(ptr, val) \
*(ptr) = val;\
*((ptr) + 1) = (val >> 8);

#define WRITE3(ptr, val) \
WRITE2(ptr, val); \
*((ptr) + 2) = (val >> 16);

#define WRITE4(ptr, val) \
WRITE3(ptr, val);\
*((ptr) + 3) = (val >> 24);

inline uint32_t read_cache(uint8_t* ptr, int len) {
		switch (len) {
				case 1:return *ptr;
				case 2:return READ2(ptr);
				case 3:return READ3(ptr);
				case 4:return READ4(ptr);
		}
		
		assert(0);
		return 0;
}

inline void write_cache(uint8_t *ptr, int len, uint32_t value) {
		switch (len) {
				case 1:*ptr = value;break;
				case 2:WRITE2(ptr, value);break;
				case 3:WRITE3(ptr, value);break;
				case 4:WRITE4(ptr, value);break;
		}

		return;

		assert(0);
}

//note!:write doesen't give cache a signal!!


extern uint32_t dram_read(hwaddr_t,size_t);
extern void dram_write(hwaddr_t,size_t,uint32_t);
extern uint32_t cache_read_l2(hwaddr_t,size_t);

typedef uint8_t Cache_Block[64];
/*
typedef struct {
		uint32_t valid_byte:1;
		uint32_t dirty_byte:1;
		uint32_t tag:19;
		uint32_t :0;
		Cache_Block cache_block;
} Cache_Line;
*/


typedef struct {
		uint8_t valid_byte;
		uint8_t dirty_byte;
		uint32_t tag;
		Cache_Block cache_block;
} Cache_Line;

typedef struct {
		Cache_Line* cache_line;
} Cache_Set;

typedef struct {
		Cache_Set* cache_set;
} Cache;

/*
typedef struct {
		uint32_t offset:6;
		uint32_t index:7;
		uint32_t tag:19;
} Cache_Addr_l1;
*/


typedef struct {
		uint8_t offset;
		uint8_t index;
		uint32_t tag;
} Cache_Addr_l1;

typedef struct {
		uint32_t offset:6;
		uint32_t index:12;
		uint32_t tag:14;
} Cache_Addr_l2;
Cache cache_l1;
Cache cache_l2;

extern uint32_t cache_get_l1(Cache_Line*,int,hwaddr_t,size_t,int*);
//extern uint32_t cache_get_l1(Cache_Line*,int,hwaddr_t,size_t);
extern uint32_t cache_get_l2(Cache_Line*,int,hwaddr_t,size_t);
extern uint32_t cache_read_l2(hwaddr_t,size_t);


void cache_reset() {
		if (cache_l1.cache_set == NULL) {
				cache_l1.cache_set = (Cache_Set*)malloc(sizeof(Cache_Set) * set_num_l1);
				int i = 0;
				for (; i < set_num_l1; ++i) {
						cache_l1.cache_set[i].cache_line = (Cache_Line*)malloc(sizeof(Cache_Line) * line_num_l1);
				}
						
				cache_l2.cache_set = (Cache_Set*)malloc(sizeof(Cache_Set) * set_num_l2);
				i = 0;
				for (; i < set_num_l2; ++i) {
						cache_l2.cache_set[i].cache_line = (Cache_Line*)malloc(sizeof(Cache_Line) * line_num_l2);
				}
		}
		int i = 0;
		for (; i < set_num_l1; ++i) {
				int j = 0;
				for (; j < line_num_l1; ++j) {
						cache_l1.cache_set[i].cache_line[j].valid_byte = 0;
				}
		}
		for (i = 0; i < set_num_l2; ++i) {
				int j = 0;
				for (; j < line_num_l2; ++j) {
						cache_l2.cache_set[i].cache_line[j].valid_byte = 0;
						cache_l2.cache_set[i].cache_line[j].dirty_byte = 0;
				}
		}
}


/**search mem in cache**/
uint32_t cache_match_l1(Cache_Set cache_set, size_t len, Cache_Addr_l1 cache_addr) {
#if cache_debug == _ON
		Log("cache_match_l1 debug: %x\n", to_addr_l1(cache_addr));
#endif

		static int last_index = 0;
		
		if (cache_set.cache_line[last_index].tag == cache_addr.tag && cache_set.cache_line[last_index].valid_byte) {
						return read_cache((cache_set.cache_line[last_index].cache_block + cache_addr.offset), len);
		}

		int i = 0;
		for (; i < line_num_l1; ++i) {
				if (cache_set.cache_line[i].tag == cache_addr.tag && cache_set.cache_line[i].valid_byte) {
						last_index = i;
						return read_cache((cache_set.cache_line[i].cache_block + cache_addr.offset), len);
				}
		}
		return cache_get_l1(cache_set.cache_line, line_num_l1, to_addr_l1(cache_addr), len, &last_index);
}

void cache_write_dram(hwaddr_t addr) {
		Cache_Addr_l2 cache_addr_l2;
		to_cache_addr_l2(addr, cache_addr_l2);
		int no = 0;
		int i = 0;

		for (; no < line_num_l2; ++no) {
				if (cache_l2.cache_set[cache_addr_l2.index].cache_line[no].tag == cache_addr_l2.tag) {
						assert(cache_l2.cache_set[cache_addr_l2.index].cache_line[no].valid_byte);
						Cache_Line cache_line_l2 = cache_l2.cache_set[cache_addr_l2.index].cache_line[no];

						hwaddr_t temp_addr = addr & 0xffffffc0;
						for (i = 0; i < 16; ++i) {
								dram_write(temp_addr + i * 4, 4, read_cache((cache_line_l2.cache_block + i * 4), 4));
						}
						break;
				}
		}
}

uint32_t cache_match_l2(Cache_Set cache_set, size_t len, Cache_Addr_l2 cache_addr) {
#if cache_debug == _ON
		Log("cache_match_l2 debug: %x\n", to_addr_l2(cache_addr));
#endif
		int i = 0;
		for (; i < line_num_l2; ++i) {
				if (cache_set.cache_line[i].valid_byte && cache_set.cache_line[i].tag == cache_addr.tag) {
						return read_cache((cache_set.cache_line[i].cache_block + cache_addr.offset), len);
				}
		}
		return cache_get_l2(cache_set.cache_line, line_num_l2, to_addr_l2(cache_addr), len);
}
/***************/



int _random() {
		static uint32_t random = 0;
		random += 186754 + 685439 * random;
		return random;
}

/*******get mem to cache********/

uint32_t cache_get_l1(Cache_Line* cache_line, int line_num, hwaddr_t addr, size_t len, int *last_index) {
#if cache_debug == _ON
		Log("cache_get_l1 debug:addr:%x\n", addr);
#endif

		Cache_Addr_l1 cache_addr;
		to_cache_addr_l1(addr, cache_addr);
		int random = _random();
		char line_no = random & 0x7;
		int num = 0;
		for (; num < line_num; ++num) {
				if (cache_line[num].valid_byte == 0 || cache_line[num].tag == cache_addr.tag) break;
		}
		
		num = (num == line_num)?line_no:num;

		*last_index = num;

		int i = 0;
		hwaddr_t addr_s = addr & 0xffffffc0;
		for (; i < 16; ++i) {
				write_cache((cache_line[num].cache_block + i * 4), 4, cache_read_l2(addr_s + i * 4, 4));
		}
		cache_line[num].tag = cache_addr.tag;
		cache_line[num].valid_byte = 1;
		return read_cache((cache_line[num].cache_block + cache_addr.offset), len);
}


uint32_t cache_get_l2(Cache_Line* cache_line, int line_num, hwaddr_t addr, size_t len) {
#if cache_debug == _ON
		Log("cache_get_l2 debug:%x\n", addr);
#endif
		Cache_Addr_l2 cache_addr;
		to_cache_addr_l2(addr, cache_addr);
		int random = _random();
		char line_no = random & 0xf;
		int num = 0;
		for (; num < line_num_l2; ++num) {
				if (cache_line[num].valid_byte == 0) break;
		}

		num = (num < line_num_l2)?num:line_no;
		int i = 0;
		hwaddr_t addr_s = addr & 0xffffffc0;
		Cache_Line cache_line_l2 = cache_l2.cache_set[cache_addr.index].cache_line[num];
		if (cache_line_l2.dirty_byte) {
				for (i = 0; i < 16; ++i) {
						dram_write(addr_s + i * 4, 4, read_cache((cache_line_l2.cache_block + i * 4), 4));
				}
		}
		for (i = 0; i < 16; ++i) {
				write_cache((cache_line[num].cache_block + i * 4), 4, dram_read(addr_s + i * 4, 4));
		}
		cache_line[num].valid_byte = 1;
		cache_line[num].tag = cache_addr.tag;
		return read_cache((cache_line[num].cache_block +cache_addr.offset), len);
}

/****/

uint32_t cache_read_l2(hwaddr_t addr, uint32_t len) {
#if cache_debug == _ON
		Log("cache_read_l2 debug:%x\n", addr);
#endif
		Cache_Addr_l2 cache_addr;
		to_cache_addr_l2(addr, cache_addr);
		uint32_t match = 0;;
		hwaddr_t addr_off = addr & 0x3f;
		if (0x3f - addr_off + 1 >= len) {
				match = cache_match_l2(cache_l2.cache_set[cache_addr.index], len, cache_addr);
		}
		else {
				hwaddr_t addr_temp = addr;
				Cache_Addr_l2 temp;
				if (len == 2) {
						match = cache_match_l2(cache_l2.cache_set[cache_addr.index], 1, cache_addr);
						addr_temp ++;
						to_cache_addr_l2(addr_temp, temp);
						match += cache_match_l2(cache_l2.cache_set[temp.index], 1, temp) << 8;
				}
				else {
						int num1 = 0x3f - addr_off + 1;
						switch(num1) {
								case 1:{
										match = cache_match_l2(cache_l2.cache_set[cache_addr.index], 1, cache_addr); 
										addr_temp ++;
										to_cache_addr_l2(addr_temp, temp);
										match += (cache_match_l2(cache_l2.cache_set[temp.index], 4, temp) & 0x00ffffff) << 8;
										break;
										}
								case 2:{
										match = cache_match_l2(cache_l2.cache_set[cache_addr.index], 2, cache_addr);
										addr_temp += 2;
										to_cache_addr_l2(addr_temp, temp);
										match += cache_match_l2(cache_l2.cache_set[temp.index], 2, temp) << 16;
										break;
								}
								case 3:{
										match = cache_match_l2(cache_l2.cache_set[cache_addr.index], 1, cache_addr);
										addr_temp += 1;
										to_cache_addr_l2(addr_temp, temp);
										match += cache_match_l2(cache_l2.cache_set[temp.index], 2, temp) << 8;
										addr_temp += 2;
										to_cache_addr_l2(addr_temp, temp);
										match += cache_match_l2(cache_l2.cache_set[temp.index], 1, temp) << 24;
										break;
								}
								default:assert(0);
						}
				}
		}
		if (addr >= 0x7ffff80 && addr < 0x7ffffc0) {
		}
		return match;
}


uint32_t cache_read_l1(hwaddr_t addr, uint32_t len) {
#if cache_debug == _ON
		Log("cache_read_l1 debug:%x\n", addr);
#endif
		Cache_Addr_l1 cache_addr;
		to_cache_addr_l1(addr, cache_addr);
		uint32_t match = 0;
		hwaddr_t addr_off = addr & 0x3f;
		if (0x3f - addr_off + 1 >= len) {
				match = cache_match_l1(cache_l1.cache_set[cache_addr.index], len, cache_addr);
		}
		else {
				hwaddr_t addr_temp = addr;
				Cache_Addr_l1 temp;
				if (len == 2) {
						match = cache_match_l1(cache_l1.cache_set[cache_addr.index], 1, cache_addr);
						addr_temp ++;
						to_cache_addr_l1(addr_temp, temp);
						match += cache_match_l1(cache_l1.cache_set[temp.index], 1, temp) << 8;
				}
				else {
						int num1 = 0x3f - addr_off + 1;
						switch(num1) {
								case 1:{
										match = cache_match_l1(cache_l1.cache_set[cache_addr.index], 1, cache_addr); 
										addr_temp ++;
										to_cache_addr_l1(addr_temp, temp);
										match += (cache_match_l1(cache_l1.cache_set[temp.index], 4, temp) & 0x00ffffff) << 8;
										break;
										}
								case 2:{
										match = cache_match_l1(cache_l1.cache_set[cache_addr.index], 2, cache_addr);
										addr_temp += 2;
										to_cache_addr_l1(addr_temp, temp);
										match += cache_match_l1(cache_l1.cache_set[temp.index], 2, temp) << 16;
										break;
								}
								case 3:{
										match = cache_match_l1(cache_l1.cache_set[cache_addr.index], 1, cache_addr);
										addr_temp += 1;
										to_cache_addr_l1(addr_temp, temp);
										match += cache_match_l1(cache_l1.cache_set[temp.index], 2, temp) << 8;
										addr_temp += 2;
										to_cache_addr_l1(addr_temp, temp);
										match += cache_match_l1(cache_l1.cache_set[temp.index], 1, temp) << 24;
										break;
								}
								default:assert(0);
						}
				}
		}

		return match;
}

//read
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//write

bool search_cache_l1(hwaddr_t addr) {
		Cache_Addr_l1 cache_addr;
		to_cache_addr_l1(addr, cache_addr);
		Cache_Set cache_set = cache_l1.cache_set[cache_addr.index];
		
		int i = 0;
		for (; i < line_num_l1; ++i) {
				if (cache_set.cache_line[i].valid_byte && cache_set.cache_line[i].tag == cache_addr.tag) return 1;
		}
		return 0;
}

bool search_cache_l2(hwaddr_t addr) {
		Cache_Addr_l2 cache_addr;
		to_cache_addr_l2(addr, cache_addr);;
		Cache_Set cache_set = cache_l2.cache_set[cache_addr.index];
		
		int i = 0;
		for (; i < line_num_l2; ++i) {
				if (cache_set.cache_line[i].valid_byte && cache_set.cache_line[i].tag == cache_addr.tag) return 1;
		}
		return 0;
}


void cache_replace_l1(hwaddr_t addr, size_t len, uint32_t value) {
#if cache_debug == _ON
		Log("cache_replace_l1 debug: %x\n", addr);
#endif

		Cache_Addr_l1 cache_addr;
		to_cache_addr_l1(addr, cache_addr);;
		int no = 0;
		for (no = 0; no < line_num_l1; ++no) {
				if (cache_l1.cache_set[cache_addr.index].cache_line[no].tag == cache_addr.tag) {
						assert(cache_l1.cache_set[cache_addr.index].cache_line[no].valid_byte);
						write_cache((cache_l1.cache_set[cache_addr.index].cache_line[no].cache_block + cache_addr.offset), len, value);
						break;
				}
		}
		assert(no < line_num_l1);
}


void cache_replace_l2(hwaddr_t addr, size_t len, uint32_t value) {
#if cache_debug == _ON
		Log("cache_replace_l2 debug %x:\n", addr);
#endif

		Cache_Addr_l2 cache_addr;
		to_cache_addr_l2(addr, cache_addr);

		int no = 0;
		for (no = 0; no < line_num_l2; ++no) {
				if (cache_l2.cache_set[cache_addr.index].cache_line[no].tag == cache_addr.tag) {
						assert(cache_l2.cache_set[cache_addr.index].cache_line[no].valid_byte);
						cache_l2.cache_set[cache_addr.index].cache_line[no].dirty_byte = 1;
								write_cache((cache_l2.cache_set[cache_addr.index].cache_line[no].cache_block + cache_addr.offset), len, value);
						break;
				}
		}
		assert(no < line_num_l2);
}




void block_replace_l2(hwaddr_t addr) {
#if cache_debug == _ON
		Log("block_replace_l2 debug %x:", addr);
#endif

		Cache_Addr_l2 cache_addr_l2;
		Cache_Addr_l1 cache_addr_l1;
		to_cache_addr_l1(addr, cache_addr_l1);
		to_cache_addr_l2(addr, cache_addr_l2);
		int no = 0;
		int i = 0;
		int random = _random();
		int no2 = random & 0xf;
		for (; no < line_num_l1; ++no) {
				if (cache_l1.cache_set[cache_addr_l1.index].cache_line[no].tag == cache_addr_l1.tag) {
						assert(cache_l1.cache_set[cache_addr_l1.index].cache_line[no].valid_byte);


						uint8_t *p1 = cache_l1.cache_set[cache_addr_l1.index].cache_line[no].cache_block;

						int empty_line = -1;
						for (i = 0; i < line_num_l2; ++i) {
								if (cache_l2.cache_set[cache_addr_l2.index].cache_line[i].tag == cache_addr_l2.tag) break;
								empty_line = (empty_line < 0 && !cache_l2.cache_set[cache_addr_l2.index].cache_line[i].valid_byte)?i:empty_line;
						}
						no2 = (empty_line < 0)?no2:empty_line;
						i = (i < line_num_l2)?i:no2;
						Cache_Line cache_line_l2 = cache_l2.cache_set[cache_addr_l2.index].cache_line[i];

						int j;
						if (cache_line_l2.valid_byte && cache_line_l2.tag != cache_addr_l2.tag) {
								hwaddr_t temp_addr = addr & 0xffffffc0;
								for (j = 0; j < 16; ++j) {
										dram_write(temp_addr + j * 4, 4, read_cache((cache_line_l2.cache_block + j * 4), 4));
								}
						}
						uint8_t *p2 = cache_l2.cache_set[cache_addr_l2.index].cache_line[i].cache_block;
						for (j = 0; j < 16; ++j) {
								write_cache((p2 + j * 4), 4, read_cache(p1 + j * 4, 4));
						}
						cache_l2.cache_set[cache_addr_l2.index].cache_line[i].tag = cache_addr_l2.tag;
						cache_l2.cache_set[cache_addr_l2.index].cache_line[i].valid_byte = 1;
						break;
				}
		}
		assert(no < line_num_l1);
}


void cache_write_l2(hwaddr_t addr,size_t len,uint32_t value) {
#if cache_debug == _ON
		Log("cache_write_l2 debug %x,%x:", addr, value);
#endif
		Cache_Addr_l2 cache_addr;
		to_cache_addr_l2(addr, cache_addr);
		if(!search_cache_l2(addr) || !search_cache_l2(addr + len - 1)) {
				cache_match_l2(cache_l2.cache_set[cache_addr.index], 1, cache_addr);
				hwaddr_t temp_addr = addr + len - 1;
				Cache_Addr_l2 temp;
				to_cache_addr_l2(temp_addr, temp);
				cache_match_l2(cache_l2.cache_set[temp.index], 1, temp);
		}
		
		int num = 0x40 - cache_addr.offset;
		
		if (num >= len) {
				cache_replace_l2(addr, len, value);
		}
		else {
				if (len == 2) {
						cache_replace_l2(addr, 1, value & 0xff);
						cache_replace_l2(addr + 1, 1, ((uint16_t)value) >> 8);
				}
				else {
						switch (num) {
								case 1:cache_replace_l2(addr, 1, value & 0xff);cache_replace_l2(addr + 1, 1, (value >> 8) & 0xff);cache_replace_l2(addr + 2, 2, ((uint32_t)value) >> 16);break;
								case 2:cache_replace_l2(addr, 2, value & 0xffff);cache_replace_l2(addr + 2, 2, ((uint32_t)value) >> 16);break;
   							    case 3:cache_replace_l2(addr, 1, value & 0xff);cache_replace_l2(addr + 1, 2, (value >> 8) & 0xffff);cache_replace_l2(addr + 3, 1, ((uint32_t)value) >> 24);break;
						}
				}
		}

}


void cache_write_l1(hwaddr_t addr,size_t len,uint32_t value) {
#if cache_debug == _ON
		Log("cache_write_l1 debug %x,%x:", addr, value);
#endif
		int num = 0;
		Cache_Addr_l1 cache_addr;
		to_cache_addr_l1(addr, cache_addr);
		num = 0x40 - cache_addr.offset;
		if(search_cache_l1(addr)) {
				if (num >= len) {
						cache_replace_l1(addr, len, value);
						block_replace_l2(addr);
				}
				else {
						if (len == 2) {
								cache_replace_l1(addr, 1, value & 0xff);
								block_replace_l2(addr);
								cache_write_l1(addr + 1, 1, ((uint16_t)value) >> 8);
						}
						else {
								switch (num) {
										case 1:cache_replace_l1(addr, 1, value & 0xff);block_replace_l2(addr);cache_write_l1(addr + 1, 1, (value >> 8) & 0xff);cache_write_l1(addr + 2, 2, ((uint32_t)value) >> 16);break;
										case 2:cache_replace_l1(addr, 2, value & 0xffff);block_replace_l2(addr);cache_write_l1(addr + 2, 2, ((uint32_t)value) >> 16);break;
										case 3:cache_replace_l1(addr, 1, value & 0xff);cache_replace_l1(addr + 1, 2, (value >> 8) & 0xffff);block_replace_l2(addr);cache_write_l1(addr + 3, 1, ((uint32_t)value) >> 24);break;
								}
						}
				}


		}
		else {
				cache_write_l2(addr, len, value);
				if (num < len) {
						//int unused;
						Cache_Addr_l1 temp_addr;
						hwaddr_t temp = addr + len - 1;
						to_cache_addr_l1(temp, temp_addr);
						cache_get_l1(cache_l1.cache_set[temp_addr.index].cache_line, line_num_l1, temp, 1, &num);
				}
		}
#ifndef RELEASE
		if (cache_read_l1(addr, len) != value) {
				Log("len :%d  cache read1 :%x cacheread2 %x\nvalue :%x", len, cache_read_l1(addr, len), cache_read_l2(addr, len), value);
								printf("\033[1;34mnum %d value %x addr %x\033[0m", num, value, addr);
				assert(0);
		}
#endif
}




