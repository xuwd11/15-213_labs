#include <stdio.h>
#include <stdlib.h>

// given an address, get the value stored at that byte
// explanation: cast to char* (address 1 byte), dereference 
#define GET_BYTE(p) (*(char*)p)

// given an address, get the value stored at word (4 bytes)
// explanation: cast to int* (address 4 bytes), dereference
#define GET_WORD(p) (*(int*)p)

// given an address and a value, write a 1-byte value at that address
// explanation: cast to a char* (address 1 byte), dereference and store
#define PUT_BYTE(p, v) (*(char*)(p) = v)

// given an address and a value, write a 4-byte value at that address
// explanation: cast to an int* (address 4 bytes), dereference and store
#define PUT_WORD(p, v) (*(int*)(p) = v)

// given an address, get the address of the next byte
// Explanation: cast to char*, then increment by 1 byte
#define GET_NEXT_BYTE_ADDR(p) ((char*)(p) + 1)

// given an address, get the value stored in the next byte
// Explanation: we can use GET_NEXT_BYTE_ADDR to get the address of the next byte, 
// then dereference the obtained address
#define GET_NEXT_BYTE_VAL(p) (*(GET_NEXT_BYTE_ADDR(p)))

// given an address, write a value to the next byte
// Explanation: we can use GET_NEXT_BYTE_ADDR to get the address of the next byte, 
// then PUT to write the value 
#define PUT_NEXT_BYTE_VAL(p, v) (PUT_BYTE(GET_NEXT_BYTE_ADDR(p), v))

int main() { 

    // use for tests with char*
    char* A = malloc(sizeof(char) * 2); 
    PUT_BYTE(A, 'a');  
    printf("%c\n", GET_BYTE(A)); // should be 'a'
    PUT_NEXT_BYTE_VAL(A, 'b'); 
    printf("%c\n", GET_NEXT_BYTE_VAL(A)); // should be 'b'
    free(A); 
      
    // use for tests with int*
    int* B = malloc(sizeof(int)); 
    PUT_WORD(B, 42); 
    printf("%d\n", GET_WORD(B)); // should be '42'
    free(B); 
    return 0; 
}
