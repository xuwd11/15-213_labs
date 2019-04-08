#include <stdbool.h>

#ifndef TYPES_H
#define TYPES_H

typedef void* elem; 
typedef bool (*are_equal)(elem e1, elem e2); 
typedef void (*map_fn)(elem e); 
typedef void (*print_elem)(elem e); 
typedef void (*elem_free)(elem e);

#endif // TYPES_H
