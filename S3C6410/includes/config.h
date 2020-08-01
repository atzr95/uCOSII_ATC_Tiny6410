#ifndef _TYPEDEF_H_
#define _TYPEDEF_H_

#define CONFIG_SERIAL1
#define CONFIG_SYS_CLK_FREQ	12000000	/* the SMDK6400 has 12MHz input clock */
#define _MMUTT_STARTADDRESS 0x57ff8000
#define _ISR_STARTADDRESS	0x57fffe00

#define _CHECK_MEM_
#define ADDR_DYNAMIC_MEM 0x53000000
#define MAX_DYNAMIC_MEM	0x4000000

#endif
