/*
 * Explicit free list from
 * https://github.com/mightydeveloper/Malloc-Lab/blob/master/mm.c
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>

#include "mm.h"
#include "memlib.h"

/*********************************************************
 * NOTE TO STUDENTS: Before you do anything else, please
 * provide your team information in the following struct.
 ********************************************************/
team_t team = {
    /* Team name */
    "xuwd11",
    /* First member's full name */
    "xuwd11",
    /* First member's email address */
    "xuwd11@gmail.com",
    /* Second member's full name (leave blank if none) */
    "",
    /* Second member's email address (leave blank if none) */
    ""
};

/* single word (4) or double word (8) alignment */
#define ALIGNMENT 8

/* rounds up to the nearest multiple of ALIGNMENT */
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~0x7)


#define SIZE_T_SIZE (ALIGN(sizeof(size_t)))


/* additional macros */
#define WSIZE 4
#define DSIZE 8
#define INITCHUNKSIZE (1<<6)
#define CHUNKSIZE (1<<12)

#define REALLOC_BUFFER (1<<7)

#define MAX(x, y) ((x) > (y) ? (x) : (y))
#define MIN(x, y) ((x) < (y) ? (x) : (y))

#define PACK(size, status) ((size) | (status))

#define GET(p) (*(unsigned int *)(p))
#define GET_SIZE(p) (GET(p) & ~0X7)
#define GET_ALLOC(p) (GET(p) & 0X1)
#define GET_RA(p) (GET(p) & 0X2)
#define SET_RA(p) (GET(p) |= 0X2)
#define REMOVE_RA(p) (GET(p) &= ~0X2)

#define PUT(p, val) (GET(p) = (val) | GET_RA(p))
#define PUT_NORA(p, val) (GET(p) = (val))

#define SET_P(p, ptr) (GET(p) = (unsigned int)(ptr))

#define HEADERP(p) ((char *)(p) - WSIZE)
#define FOOTERP(p) ((char *)(p) + GET_SIZE(HEADERP(p)) - DSIZE)

#define PREV_BLKP(p) ((char *)(p) - GET_SIZE((char *)(p) - DSIZE))
#define NEXT_BLKP(p) ((char *)(p) + GET_SIZE((char *)(p) - WSIZE))

#define PRED_P(p) ((char *)(p))
#define SUCC_P(p) ((char *)(p) + WSIZE)

#define PRED(p) (*(char **)(p))
#define SUCC(p) (*(char **)(SUCC_P(p)))


/* free list */
void *free_list;


/* 
 * mm_init - initialize the malloc package.
 */
int mm_init(void)
{
    return 0;
}

/* 
 * mm_malloc - Allocate a block by incrementing the brk pointer.
 *     Always allocate a block whose size is a multiple of the alignment.
 */
void *mm_malloc(size_t size)
{
    int newsize = ALIGN(size + SIZE_T_SIZE);
    void *p = mem_sbrk(newsize);
    if (p == (void *)-1)
	return NULL;
    else {
        *(size_t *)p = size;
        return (void *)((char *)p + SIZE_T_SIZE);
    }
}

/*
 * mm_free - Freeing a block does nothing.
 */
void mm_free(void *ptr)
{
}

/*
 * mm_realloc - Implemented simply in terms of mm_malloc and mm_free
 */
void *mm_realloc(void *ptr, size_t size)
{
    void *oldptr = ptr;
    void *newptr;
    size_t copySize;
    
    newptr = mm_malloc(size);
    if (newptr == NULL)
      return NULL;
    copySize = *(size_t *)((char *)oldptr - SIZE_T_SIZE);
    if (size < copySize)
      copySize = size;
    memcpy(newptr, oldptr, copySize);
    mm_free(oldptr);
    return newptr;
}














