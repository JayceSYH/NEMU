typedef union {
    struct {
		uint32_t limit_15_0			:16;
		uint32_t base_15_0			:16;
		uint32_t base_23_16			:8;
		uint32_t type				:4;
		uint32_t segment_type		:1;
		uint32_t prililege_level	:2;
		uint32_t present			:1;
		uint32_t limit_19_16		:4;
		uint32_t soft_use			:1;
		uint32_t operation_size		:1;
		uint32_t pad0				:1;
		uint32_t granularity		:1;
		uint32_t base_31_24			:8;
    };
    unsigned long long val;
} SegDesc;

typedef union {
    struct {
				uint32_t offset_15_0		:16;
				uint32_t segment			:16;
				uint32_t pad0				:8;
				uint32_t type				:4;
				uint32_t system				:1;
				uint32_t privilege_level	:2;
				uint32_t present			:1;
				uint32_t offset_31_16		:16;
            };
    unsigned long long  val;
} GateDesc;

void sreg_update(int);
