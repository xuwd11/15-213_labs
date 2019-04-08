#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

/**********************************/
/* Data Types */
/**********************************/

struct person_node { 
    char* name;
    int age;
};

struct pet_node { 
    char* name; 
    char type[10]; 
};

typedef struct person_node person; 
typedef struct pet_node* pet; 

// Note: all these solutions use malloc; it's perfectly acceptable to 
// use calloc too (and it may be even more intuitive, since calloc usage
// generally implies that the allocated memory block will be used as an array)
// In general, you should be fine using either malloc or calloc, so long 
// as you know that malloc doesn't initialize memory to 0's, while calloc does
// (for this reason, calloc performs slightly slower than malloc, since it's 
// essentially a malloc followed by a memset to 0's)
int main() {

    /* TODO: dynamically allocate enough memory, assign the blocks to these pointers */

    /*************************************/

    int* A;     // Construct a 10-element integer array 
    A = malloc(sizeof(int) * 10); 

    /*************************************/   

    int** B;    // Construct a 2D array of 10 x 10 integers
    B = malloc(sizeof(int*) * 10); // allocates array of int* (to each array)
    for (int i = 0; i < 10; i++) { 
        B[i] = malloc(sizeof(int) * 10); // allocates array of int's
    }

    /*************************************/   

    person* C;  // Construct an array of 10 persons, allocate 10 characters for each name
    
    // note the usage of sizeof(person); in this case, struct person_node has been 
    // typedef'd to person, so sizeof(person) allocates the correct amount of memory
    // for each struct 
    C = malloc(sizeof(person) * 10); 
    for (int i = 0; i < 10; i++) { 
        // C is of type person*.  So, C[i] is of type person, which we typedef'd as a 
        // struct person.  So, we use the '.' operator to access the fields of C[i]
        C[i].name = malloc(sizeof(char) * 10); 
    }

    /*************************************/   

    pet* D;     // Construct an array of 10 pets, allocate 10 characters for each name;

    // here, we want an array of 10 elements of type pet.  pet is simply typedef'd to 
    // struct pet_node*.  So, the allocation simply allocates memory for 10 struct pointers 
    D = malloc(sizeof(pet) * 10);
    for (int i = 0; i < 10; i++) { 
        // remember that D[i] is a struct pet_node* type; we need to allocate memory for 
        // the struct that it points to
        D[i] = malloc(sizeof(struct pet_node) * 10);  

        // Since D[i] is of type struct pet_node*, we use the '->' operator to access the fields of D[i]

        // note: the '[]' and the '->' operators both derefernce memory; parentheses can clarify 
        // exactly which should happen first (don't rely upon order of precedence for 
        // something like this)
        (D[i])->name = malloc(sizeof(char) * 10);   
    } 

    /*************************************/   

    /* TODO: free any memory you just allocated before you return! */

    /*************************************/   

    free(A); // simple; we only malloc'd A and didn't modify anything else

    /*************************************/   

    // to free B properly, we should first free the sub-arrays that we allocated, then 
    // the array of pointers to those sub-arrays.  If we first free B, we won't be able 
    // to access the arrays indexed at B[i] (you can't dereference memory that you free'd)
    // This is a typical approach; we allocate memory top-down and we free memory bottom-up 
    for (int i = 0; i < 10; i++) { 
        free(B[i]);
    } 
    free(B);

    /*************************************/   

    // now, we need to free the data stored in each struct; the same 'free bottom-up' 
    // approach is applicable again
    for (int i = 0; i < 10; i++) { 
        free(C[i].name); 
    }
    free(C); 

    /*************************************/   

    for (int i = 0; i < 10; i++) { 
        // be careful; we did not allocate memory for type, so we can NOT free the 
        // type array in the pet_node struct.  
        free((D[i])->name); 

        // Also, unlike for for the last problem  (where C[i] was just a struct, 
        // which we had allocated memory for by virtue of allocating memory for C), 
        // D[i] is just a pointer to another malloc'd memory block.  So, we must free D[i] too
        // (but again, only after we've free'd name!)
        free(D[i]); 
    }
    free(D); 

    /*************************************/   

    return 0; 
}
