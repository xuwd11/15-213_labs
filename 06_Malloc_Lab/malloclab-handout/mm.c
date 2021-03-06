/*
 * Explicit free list
 * in https://github.com/mightydeveloper/Malloc-Lab/blob/master/mm.c
 *
 * 98/100 was achieved without segregated free list
 */

/***********************************************************************************************************
    Block information (adapted from https://github.com/mightydeveloper/Malloc-Lab/blob/master/mm.c)
 
A   : Allocated? (1: true, 0:false)
RA  : Reallocation tag (1: true, 0:false)
 
 < Allocated Block >
 
 
             31 30 29 28 27 26 25 24 23 22 21 20 19 18 17 16 15 14 13 12 11 10  9  8  7  6  5  4  3  2  1  0
            +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 Header :   |                              size of the block                                       |  |  | A|
   ptr ---> +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
            |                                                                                               |
            |                                                                                               |
            .                              Payload and padding                                              .
            .                                                                                               .
            .                                                                                               .
            +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 Footer :   |                              size of the block                                       |     | A|
            +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 
 
 < Free block >
 
             31 30 29 28 27 26 25 24 23 22 21 20 19 18 17 16 15 14 13 12 11 10  9  8  7  6  5  4  3  2  1  0
            +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 Header :   |                              size of the block                                       |  |RA| A|
   ptr ---> +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
            |                 pointer to the next free block in free list                                   |
ptr+WSIZE-> +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
            |                 pointer to the previous free block in free list                               |
            +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
            .                                                                                               .
            .                                                                                               .
            .                                                                                               .
            +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 Footer :   |                              size of the block                                       |     | A|
            +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 
 
************************************************************************************************************/

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

#define GET(ptr) (*(unsigned int *)(ptr))
#define GET_SIZE(ptr) (GET(ptr) & ~0X7)
#define GET_ALLOC(ptr) (GET(ptr) & 0X1)
#define GET_RA(ptr) (GET(ptr) & 0X2)
#define SET_RA(ptr) (GET(ptr) |= 0X2)
#define REMOVE_RA(ptr) (GET(ptr) &= ~0X2)

#define PUT(ptr, val) (GET(ptr) = (val) | GET_RA(ptr))
#define PUT_NORA(ptr, val) (GET(ptr) = (val))

#define SET_PTR(ptr, ptr2) (GET(ptr) = (unsigned int)(ptr2))

#define HEADERP(ptr) ((char *)(ptr) - WSIZE)
#define FOOTERP(ptr) ((char *)(ptr) + GET_SIZE(HEADERP(ptr)) - DSIZE)

// Address of physically next and previous blocks
#define NEXT_BLKP(ptr) ((char *)(ptr) + GET_SIZE((char *)(ptr) - WSIZE))
#define PREV_BLKP(ptr) ((char *)(ptr) - GET_SIZE((char *)(ptr) - DSIZE))

// Pointers that store the address of next and previous blocks in a free block
#define NEXT_PTR(ptr) ((char *)(ptr))
#define PREV_PTR(ptr) ((char *)(ptr) + WSIZE)

// Address of the next and previous blocks in the free list
#define NEXT(ptr) (*(char **)(ptr))
#define PREV(ptr) (*(char **)(PREV_PTR(ptr)))


/* free list */
void *free_list;


/* Helper functions */
static void *extend_heap(size_t size);
static void *coalesce(void *ptr);
static void *place(void *ptr, size_t asize);
static void insert_node(void *ptr, size_t size);
static void delete_node(void *ptr);

static void insert_node(void *ptr, size_t size) {
    void *right = free_list;
    void *left = NULL;
    // keep the size of free blocks in a non-decreasing order
    while ((right != NULL) && (size > GET_SIZE(HEADERP(right)))) {
        left = right;
        right = NEXT(right);
    }
    if (right != NULL) {
        if (left != NULL) { // insert between 2 nodes
            SET_PTR(PREV_PTR(ptr), left);
            SET_PTR(NEXT_PTR(left), ptr);
            SET_PTR(PREV_PTR(right), ptr);
            SET_PTR(NEXT_PTR(ptr), right);
        }
        else { // insert into the beginning of the list
            SET_PTR(NEXT_PTR(ptr), right);
            SET_PTR(PREV_PTR(right), ptr);
            SET_PTR(PREV_PTR(ptr), NULL);
            free_list = ptr;
        }
    }
    else {
        if (left != NULL) { //insert into the end of the list
            SET_PTR(NEXT_PTR(left), ptr);
            SET_PTR(PREV_PTR(ptr), left);
            SET_PTR(NEXT_PTR(ptr), NULL);
        }
        else { // initialize from an empty free list
            SET_PTR(NEXT_PTR(ptr), NULL);
            SET_PTR(PREV_PTR(ptr), NULL);
            free_list = ptr;
        }
    }
    return;
}

static void delete_node(void *ptr) {
    if (PREV(ptr) != NULL) {
        if (NEXT(ptr) != NULL) { // between 2 nodes
            SET_PTR(NEXT_PTR(PREV(ptr)), NEXT(ptr));
            SET_PTR(PREV_PTR(NEXT(ptr)), PREV(ptr));
        }
        else { // at the end of the free list
            SET_PTR(NEXT_PTR(PREV(ptr)), NULL);
        }
    }
    else {
        if (NEXT(ptr) != NULL) { // at the beginning of the free list
            SET_PTR(PREV_PTR(NEXT(ptr)), NULL);
            free_list = NEXT(ptr);
        }
        else // the only node in the free list
            free_list = NULL;
    }
    return;
}

static void *coalesce(void *ptr) {
    size_t prev_alloc = GET_ALLOC(HEADERP(PREV_BLKP(ptr)));
    size_t next_alloc = GET_ALLOC(HEADERP(NEXT_BLKP(ptr)));
    size_t size = GET_SIZE(HEADERP(ptr));

    if (GET_RA(HEADERP(PREV_BLKP(ptr))) == 1)
        prev_alloc = 1;
    
    if (prev_alloc == 1 && next_alloc == 1) // cannot coalesce with previous or next block
        return ptr;
    else if (prev_alloc == 1 && next_alloc == 0) { // can coalesce with next block
        delete_node(ptr);
        delete_node(NEXT_BLKP(ptr));
        size += GET_SIZE(HEADERP(NEXT_BLKP(ptr)));
        PUT(HEADERP(ptr), PACK(size, 0));
        PUT(FOOTERP(ptr), PACK(size, 0));
    }
    else if (prev_alloc == 0 && next_alloc == 1) { // can coalesce with previous block
        delete_node(ptr);
        delete_node(PREV_BLKP(ptr));
        size += GET_SIZE(HEADERP(PREV_BLKP(ptr)));
        ptr = PREV_BLKP(ptr);
        PUT(HEADERP(ptr), PACK(size, 0));
        PUT(FOOTERP(ptr), PACK(size, 0));
    }
    else if (prev_alloc == 0 && next_alloc == 0) { // can coalesce with both previous and next blocks
        delete_node(ptr);
        delete_node(PREV_BLKP(ptr));
        delete_node(NEXT_BLKP(ptr));
        size += (GET_SIZE(HEADERP(NEXT_BLKP(ptr))) + GET_SIZE(HEADERP(PREV_BLKP(ptr))));
        ptr = PREV_BLKP(ptr);
        PUT(HEADERP(ptr), PACK(size, 0));
        PUT(FOOTERP(ptr), PACK(size, 0));
    }
    insert_node(ptr, size);
    return ptr;
}

static void *extend_heap(size_t size) {
    void *ptr;
    size_t asize;
    asize = ALIGN(size);

    if ((ptr = mem_sbrk(asize)) == (void *)-1)
        return NULL;
    
    PUT_NORA(HEADERP(ptr), PACK(asize, 0));
    PUT_NORA(FOOTERP(ptr), PACK(asize, 0));
    PUT_NORA(HEADERP(NEXT_BLKP(ptr)), PACK(0, 1));
    insert_node(ptr, asize);

    return coalesce(ptr);
}

static void *place(void *ptr, size_t asize) {
    size_t psize = GET_SIZE(HEADERP(ptr));
    size_t remainder = psize - asize;

    delete_node(ptr);

    if (remainder <= DSIZE * 2) { // don't split
        PUT(HEADERP(ptr), PACK(psize, 1));
        PUT(FOOTERP(ptr), PACK(psize, 1));
    }
    else if (asize >= 100) { // split
        PUT(HEADERP(ptr), PACK(remainder, 0));
        PUT(FOOTERP(ptr), PACK(remainder, 0));
        PUT_NORA(HEADERP(NEXT_BLKP(ptr)), PACK(asize, 1));
        PUT_NORA(FOOTERP(NEXT_BLKP(ptr)), PACK(asize, 1));
        insert_node(ptr, remainder);
        return NEXT_BLKP(ptr);
    }
    else { //split
        PUT(HEADERP(ptr), PACK(asize, 1));
        PUT(FOOTERP(ptr), PACK(asize, 1));
        PUT_NORA(HEADERP(NEXT_BLKP(ptr)), PACK(remainder, 0));
        PUT_NORA(FOOTERP(NEXT_BLKP(ptr)), PACK(remainder, 0));
        insert_node(NEXT_BLKP(ptr), remainder);
    }
    return ptr;
}


/* 
 * mm_init - initialize the malloc package.
 * reset free list and initialize heap area
 */
int mm_init(void)
{
    free_list = NULL;
    char *heap;
    if ((long)(heap = mem_sbrk(4 * WSIZE)) == -1)
        return -1;
    PUT_NORA(heap, 0); // alignment padding
    PUT_NORA(heap + (1 * WSIZE), PACK(DSIZE, 1)); // prologue header
    PUT_NORA(heap + (2 * WSIZE), PACK(DSIZE, 1)); // prologue footer
    PUT_NORA(heap + (3 * WSIZE), PACK(0, 1)); // epilogue header
    if (extend_heap(INITCHUNKSIZE) == NULL)
        return -1;
    return 0;
}

/* 
 * mm_malloc - Allocate a block by incrementing the brk pointer.
 *     Always allocate a block whose size is a multiple of the alignment.
 */
void *mm_malloc(size_t size)
{
    if (size == 0)
        return NULL;
    
    void *ptr = NULL;
    size_t asize;
    size_t extendsize;

    if (size < DSIZE)
        asize = 2 * DSIZE;
    else
        asize = ALIGN(size + DSIZE);
    
    ptr = free_list;
    while ((ptr != NULL) && ((asize > GET_SIZE(HEADERP(ptr))) || (GET_RA(HEADERP(ptr)))))
        ptr = NEXT(ptr);
    if (ptr == NULL) {
        extendsize = MAX(asize, CHUNKSIZE);
        if ((ptr = extend_heap(extendsize)) == NULL)
            return NULL;
    }

    ptr = place(ptr, asize);

    return ptr;
}

/*
 * mm_free - Freeing a block does nothing.
 */
void mm_free(void *ptr)
{
    size_t size = GET_SIZE(HEADERP(ptr));
    REMOVE_RA(HEADERP(NEXT_BLKP(ptr)));
    PUT(HEADERP(ptr), PACK(size, 0));
    PUT(FOOTERP(ptr), PACK(size, 0));
    insert_node(ptr, size);
    coalesce(ptr);
    return;
}

/*
 * mm_realloc - Implemented simply in terms of mm_malloc and mm_free
 */
void *mm_realloc(void *ptr, size_t size)
{
    if (ptr == NULL)
        return mm_malloc(size);

    if (size == 0)
        return NULL;
    
    void *newptr = ptr;
    size_t newsize = size;
    int remainder, extendsize, blockbuffer;

    if (newsize < DSIZE)
        newsize = 2 * DSIZE;
    else
        newsize = ALIGN(size + DSIZE);
    
    newsize += REALLOC_BUFFER;

    blockbuffer = GET_SIZE(HEADERP(ptr)) - newsize;

    if (blockbuffer < 0) {
        if (GET_ALLOC(HEADERP(NEXT_BLKP(ptr))) == 0 || GET_SIZE(HEADERP(NEXT_BLKP(ptr))) == 0) {
            remainder = GET_SIZE(HEADERP(ptr)) + GET_SIZE(HEADERP(NEXT_BLKP(ptr))) - newsize;
            if (remainder < 0) {
                extendsize = MAX(-remainder, CHUNKSIZE);
                if (extend_heap(extendsize) == NULL)
                    return NULL;
                remainder += extendsize;
            }

            delete_node(NEXT_BLKP(ptr));

            PUT_NORA(HEADERP(ptr), PACK(newsize + remainder, 1));
            PUT_NORA(FOOTERP(ptr), PACK(newsize + remainder, 1));
        }
        else {
            newptr = mm_malloc(newsize - DSIZE);
            memcpy(newptr, ptr, MIN(size, newsize));
            mm_free(ptr);
        }
        blockbuffer = GET_SIZE(HEADERP(newptr)) - newsize;
    }

    if (blockbuffer < 2 * REALLOC_BUFFER)
        SET_RA(HEADERP(NEXT_BLKP(newptr)));

    return newptr;
}