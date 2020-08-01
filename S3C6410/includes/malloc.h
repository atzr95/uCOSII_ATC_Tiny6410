#ifndef _MALLOC_H_
#define _MALLOC_H_

void free_mem(void *base);
void *alloc_mem(unsigned int nByte);
void *realloc_mem(void *base, unsigned int nByte);
void mem_init(void);
void check_memory(void);
void *copy_mem(void *dest, const void *src, int n);

#endif /* _MALLOC_H_ */
