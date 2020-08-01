#include <stdio.h>
#include "..\includes\config.h"
#include "..\includes\uart.h"
#include "..\..\uCOS_II\SOURCE\ucos_ii.h"

#define NR_HOLES         128 * 64    /* max entries in hole table */
#define SIZE_EXTRA		4
#define NIL_HOLE (hole *)0

#define TOHEAD(X) ((unsigned char *)((unsigned int *)(X)-1))
#define TOADDR(X) ((void*)((unsigned int *)X+1))
#define SET_SIZE(P,X) (*((unsigned int *)P) = X)
#define GET_SIZE(P) (*((unsigned int *)TOHEAD(P)))
#define ROUNDUP(X)  ((X+3) & ~3)

#ifdef _CHECK_MEM_
unsigned int nByteUsedMem = 0;
unsigned int nByteUsedMemBak = 0;
#endif

typedef struct hole {
	unsigned char *h_base;
	unsigned int h_len;
	struct hole *h_next;
} hole;

static OS_EVENT *MemAllocSem;

static hole ahole[NR_HOLES];
static hole *hole_head;		/* pointer to first hole */
static hole *free_slots;	/* ptr to list of unused table slots */

static void del_slot(hole *prev_ptr, hole *hp){
	/* prev_ptr:	pointer to hole entry just ahead of 'hp' */
	/* hp:	pointer to hole entry to be removed */

	/* Remove an entry from the hole list.  This procedure is called when a
	** request to allocate memory removes a hole in its entirety, thus reducing
	** the numbers of holes in memory, and requiring the elimination of one
	** entry in the hole list.
	*/
	if (hp == hole_head){
		hole_head = hp->h_next;
	}
	else{
		prev_ptr->h_next = hp->h_next;
	}
	hp->h_next = free_slots;
	free_slots = hp;
}


static void merge(hole *hp)
/* hp: ptr to hole to merge with its successors */
{
	hole *next_ptr;
	/* If 'hp' points to the last hole, no merging is possible.  If it does not,
	** try to absorb its successor into it and free the successor's table entry.
	*/
	if ( (next_ptr = hp->h_next) == NIL_HOLE) return;
	if (hp->h_base + hp->h_len == next_ptr->h_base) {
		hp->h_len += next_ptr->h_len;   /* first one gets second one's mem */
		del_slot(hp, next_ptr);
	} else {
		hp = next_ptr;
	}
	/* If 'hp' now points to the last hole, return; otherwise, try to absorb its
	** successor into it.
	*/
	if ( (next_ptr = hp->h_next) == NIL_HOLE) return;
	if (hp->h_base + hp->h_len == next_ptr->h_base) {
		hp->h_len += next_ptr->h_len;
		del_slot(hp, next_ptr);
	}
}

void free_mem(void *base){
	/* Return a block of free memory to the hole list.  The parameters tell where
	** the block starts in physical memory and how big it is.  The block is added
	** to the hole list.  If it is contiguous with an existing hole on either end,
	** it is merged with the hole or holes.
	*/
	INT8U err;

	hole *hp, *new_ptr, *prev_ptr;
	unsigned int nByte;

	nByte = GET_SIZE(base);
	if (nByte == 0) return;

	OSSemPend(MemAllocSem, 0, &err);

#ifdef _CHECK_MEM_
	if(nByteUsedMemBak < nByteUsedMem){
		nByteUsedMemBak = nByteUsedMem;
	}
	nByteUsedMem -= nByte;
#endif

	if ( (new_ptr = free_slots) == NIL_HOLE){
		// assert(0);
		OSSemPost(MemAllocSem);
		return ;
	}
	new_ptr->h_base = TOHEAD(base);
	new_ptr->h_len = nByte;
	free_slots = new_ptr->h_next;
	hp = hole_head;
	if (hp == NIL_HOLE || TOHEAD(base) <= hp->h_base) {
		new_ptr->h_next = hp;
		hole_head = new_ptr;
		merge(new_ptr);
	}else{
		while (hp != NIL_HOLE && TOHEAD(base) > hp->h_base) {
			prev_ptr = hp;
			hp = hp->h_next;
		}

		new_ptr->h_next = prev_ptr->h_next;
		prev_ptr->h_next = new_ptr;
		merge(prev_ptr);              /* sequence is 'prev_ptr', 'new_ptr', 'hp' */
	}
	OSSemPost(MemAllocSem);
}

void *alloc_mem(unsigned int nByte){
	/* Allocate a block of memory from the free list using first fit. The block
	** consists of a sequence of contiguous bytes, whose length in clicks is
	** given by 'clicks'.  A pointer to the block is returned.  The block is
	** always on a click boundary.  This procedure is called when memory is
	** needed for FORK or EXEC.
	*/
	INT8U err;

	hole *hp, *prev_ptr;
	unsigned char *base;
	hp = hole_head;

	nByte += SIZE_EXTRA;
	nByte = ROUNDUP(nByte);
	OSSemPend(MemAllocSem, 0, &err);
	while (hp != NIL_HOLE) {
		if (hp->h_len >= nByte) {
			/* We found a hole that is big enough.  Use it. */

			nByteUsedMem += nByte;

			base = hp->h_base;
			hp->h_base += nByte;
			hp->h_len -= nByte;

			if (hp->h_len != 0){
				SET_SIZE(base,nByte);
				OSSemPost(MemAllocSem);
				return TOADDR(base);
			}
			/* add it to free list*/
			del_slot(prev_ptr, hp);

			SET_SIZE(base,nByte);
			OSSemPost(MemAllocSem);
			return TOADDR(base);
		}
		prev_ptr = hp;
		hp = hp->h_next;
	}
	OSSemPost(MemAllocSem);
	return 0;
}

/* unoptimized memory copy */
void *copy_mem(void *dest, const void *src, int n)
{
	const unsigned char *s = (const unsigned char *)src;
	unsigned char *d = (unsigned char *)dest;

	while(n-- > 0)
		*d++ = *s++;

	return dest;
}

void *realloc_mem(void *base, unsigned int nByte){
	unsigned char *ptr;
	unsigned int old_size;

	if(base == 0){
		return alloc_mem(nByte);
	}
	if(nByte == 0){
		free_mem(base);
		return 0;
	}
	old_size = GET_SIZE(base)-SIZE_EXTRA;
	ptr = alloc_mem(nByte+old_size);
	if(ptr == 0){
		return 0;
	}
	copy_mem(ptr,base,old_size);
	free_mem(base);
	return ptr;
}


unsigned int max_hole()
{
	/* Scan the hole list and return the largest hole. */
	hole *hp;
	unsigned int max;
	hp = hole_head;
	max = 0;
	while (hp != NIL_HOLE) {
		if (hp->h_len > max) max = hp->h_len;
		hp = hp->h_next;
	}
	return max;
}

void mem_init(void)
{
	hole *hp;
	unsigned char *base = (unsigned char *)ADDR_DYNAMIC_MEM;

	MemAllocSem = OSSemCreate(1);

	for (hp = &ahole[0]; hp < &ahole[NR_HOLES]; hp++){
		hp->h_len = 0;
		hp->h_base = 0;
		hp->h_next = hp + 1;
	}
	ahole[NR_HOLES-1].h_next = NIL_HOLE;
	ahole[NR_HOLES-1].h_base = 0;
	ahole[NR_HOLES-1].h_len = 0;

	hole_head = NIL_HOLE;
	free_slots= &ahole[0];
	SET_SIZE(base,MAX_DYNAMIC_MEM);
	free_mem(TOADDR(base));
#ifdef _CHECK_MEM_
	nByteUsedMem = 0;
	nByteUsedMemBak = 0;
#endif
}

void check_memory(void){

#ifdef _CHECK_MEM_

	hole *hp = hole_head;
	hole *fr = free_slots;

	Uart_Printf("\nbase mem addr: %x\n", ADDR_DYNAMIC_MEM);
	Uart_Printf("hole_head addr: %x\n",hp);
	Uart_Printf("free_slots addr: %x\n",fr);
	Uart_Printf("max alloc mem: %x bytes\n",nByteUsedMemBak);
	Uart_Printf("unrelease mem: %x bytes\n",nByteUsedMem);
	while(hp != NIL_HOLE){
		Uart_Printf("hp->h_base: %x\t",hp->h_base);
		Uart_Printf("hp->h_len: %x\t",hp->h_len);
		Uart_Printf("hp->h_next: %x\n",hp->h_next);

		hp = hp->h_next;
	}
	while(fr != NIL_HOLE && fr->h_len != 0){
		Uart_Printf("fr->h_base: %x\t",fr->h_base);
		Uart_Printf("fr->h_len: %x\t",fr->h_len);
		Uart_Printf("fr->h_next: %x\n",fr->h_next);

		fr = fr->h_next;
	}
#endif
}


extern char heap_start;
extern char heap_end;
char *current_heap_end = 0;

void *_sbrk( int incr )
{
        char *prev_heap_end;

        if( current_heap_end == 0 ) current_heap_end = &heap_start;
        char *new_heap_end = current_heap_end + incr;

        if( new_heap_end >= &heap_start && new_heap_end <= &heap_end )
        {
                prev_heap_end = current_heap_end;
                current_heap_end = new_heap_end;

                //AddMallocRecord( 0, (dword) prev_heap_end, incr );
                return (void *) prev_heap_end;
        }

        return (void *) -1;
}
