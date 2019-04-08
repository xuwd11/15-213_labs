/* TODO: Implement these macros as specified.  In all cases, you may NOT 
 * assume anything about the ptr type passed to the macro (it may be a char*, an int*, 
 * a void**, etc) */

#include <stdio.h>
#include <stdlib.h>

// given an address, get the value stored at that byte 
#define GET_BYTE(p) (*(char*)p)

// given an address, get the value stored at word (4 bytes)
#define GET_WORD(p) (*(int*)p)

// given an address and a value, write a 1-byte value at that address
#define PUT_BYTE(p, v) (*(char*)(p) = v)

// given an address and a value, write a 4-byte value at that address
#define PUT_WORD(p, v) (*(int*)(p) = v)

// given an address, get the address of the next byte
#define GET_NEXT_BYTE_ADDR(p) ((char*)(p) + 1)

// given an address, get the value stored in the next byte
#define GET_NEXT_BYTE_VAL(p) (*(GET_NEXT_BYTE_ADDR(p)))

// given an address, write a value to the next byte
#define PUT_NEXT_BYTE_VAL(p, v) (PUT_BYTE(GET_NEXT_BYTE_ADDR(p), v))


int main() { 
    /* TODO: write test cases to ensure that your implementation works correctly
     * A good test may simply create an array of bytes, assign the elements some 
     * values and access the elemnets using the macros you've defined */
    char* A = malloc(sizeof(char) * 2);
    PUT_BYTE(A, 't');
    printf("%c\n", GET_BYTE(A));
    PUT_NEXT_BYTE_VAL(A, 'c');
    printf("%c\n", GET_NEXT_BYTE_VAL(A));
    free(A);

    int* B = malloc(sizeof(int));
    PUT_WORD(B, 1000);
    printf("%d\n", GET_WORD(B));
    free(B);

    return 0;  
}
