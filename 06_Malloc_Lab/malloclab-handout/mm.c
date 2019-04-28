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
    // keep the size of free blocks in a non-increasing order
    while ((right != NULL) && (size < GET_SIZE(HEADERP(right)))) {
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














