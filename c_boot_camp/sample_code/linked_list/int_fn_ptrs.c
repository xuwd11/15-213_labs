#include "int_fn_ptrs.h"

void print_int(elem e) { 
    REQUIRES(e != NULL);
    int val = *(int*)(e); 
    printf("%d ", val);  
}

bool ints_equal(elem e1, elem e2) { 
    REQUIRES(e1 != NULL && e2 != NULL); 
    /* TODO: Implement this */
    return false; 
}

void add_1(elem e) {
   REQUIRES(e != NULL);
   int val = *(int*)(e);  
   *(int*)(e) = val + 1;
}

void square(elem e) { 
    REQUIRES(e != NULL); 
    int val = *(int*)e;
    *(int*)(e) = val * val;  
}

void next_pow_2(elem e) { 
    REQUIRES(e != NULL);
    /* TODO: Implement this */
}
