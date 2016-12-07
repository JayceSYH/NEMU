#include "../common.h"

typedef union {
		struct {
				uint32_t present			:1;
				uint32_t read_write			:1;
				uint32_t user_supervisor	:1;
				uint32_t page_write_through	:1;
				uint32_t page_cache_disable	:1;
				uint32_t accessed			:1;
				uint32_t dirt				:1;
				uint32_t pad0				:1;
				uint32_t global				:1;
				uint32_t pad1				:3;
				uint32_t page_frame			:20;
		};
		uint32_t val;
} PTE;

typedef union {
		struct {
				uint32_t present			:1;
				uint32_t read_write			:1;
				uint32_t user_supervisor	:1;
				uint32_t page_write_through	:1;
				uint32_t page_cache_disable	:1;
				uint32_t accessed			:1;
				uint32_t pad0				:6;
				uint32_t page_frame			:20;
		};
		uint32_t val;
} PDE;
