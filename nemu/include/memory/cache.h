#ifndef _CACHE_H_
#define _CACHE_H_

uint32_t cache_read_l1(swaddr_t,size_t);
uint32_t cache_write_l1(swaddr_t,size_t,uint32_t);

#endif
