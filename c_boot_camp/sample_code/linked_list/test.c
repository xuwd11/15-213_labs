#include "list.h"
#include "int_fn_ptrs.h"

int main() { 
    list l = new_list(NULL, &print_int, &ints_equal);
    int x = 42; 
    int y = 54; 
    int z = 100; 
    int w = 13;

    append_node(&x, l); 
    append_node(&y, l); 

    prepend_node(&z, l); 
    prepend_node(&w, l);

    delete_nodes(&x, l); 
    map(l, &next_pow_2); 
    print_list(l);
    free_list(l);  
}
