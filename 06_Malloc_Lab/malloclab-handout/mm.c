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

#define PREV_BLKP(ptr) ((char *)(ptr) - GET_SIZE((char *)(ptr) - DSIZE))
#define NEXT_BLKP(ptr) ((char *)(ptr) + GET_SIZE((char *)(ptr) - WSIZE))

#define PRED_PTR(ptr) ((char *)(ptr))
#define SUCC_PTR(ptr) ((char *)(ptr) + WSIZE)

#define PRED(ptr) (*(char **)(ptr))
#define SUCC(ptr) (*(char **)(SUCC_PTR(ptr)))


/* free list */
void *free_list;


/* Helper functions */
static void *extend_heap(size_t size);
static void *coalesce(void *ptr);
static void *place(void *ptr, size_t asize);
static void insert_node(void *ptr, size_t size);
static void delete_node(void *ptr);

static void insert_node(void *ptr, size_t size) {
    void *left = free_list;
    void *right = NULL;
    // keep the size in a non-decreasing order, and set the free_list to the rightmost node
    while ((left != NULL) && (size < GET_SIZE(HEADERP(left)))) {
        right = left;
        left = PRED(left);
    }
    if (left != NULL) {
        if (right != NULL) { // insert between 2 nodes
            SET_PTR(PRED_PTR(ptr), left);
            SET_PTR(SUCC_PTR(left), ptr);
            SET_PTR(PRED_PTR(right), ptr);
            SET_PTR(SUCC_PTR(ptr), right);
        }
        else { // insert into the rightmost of the list
            SET_PTR(PRED_PTR(ptr), left);
            SET_PTR(SUCC_PTR(left), ptr);
            SET_PTR(SUCC_PTR(ptr), NULL);
            free_list = ptr;
        }
    }
    else {
        if (right != NULL) { //insert into the leftmost of the list
            SET_PTR(PRED_PTR(right), ptr);
            SET_PTR(SUCC_PTR(ptr), right);
            SET_PTR(PRED_PTR(ptr), NULL);
        }
        else { // initialize empty free list
            SET_PTR(PRED_PTR(ptr), NULL);
            SET_PTR(SUCC_PTR(ptr), NULL);
            free_list = ptr;
        }
    }
    return;
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














