#include <stdlib.h> 
#include <stdio.h>
#include <string.h>
#include "types.h"
#include "xalloc.h"
#include "contracts.h"
#include <stdbool.h>

#ifndef LIST_H
#define LIST_H

/**************************/
/* Node Types */
/**************************/
typedef struct list_node* node;
typedef struct linked_list* list; 

struct list_node { 
    elem data; 
    node next;
    node prev;  
};

struct linked_list { 
    node start; 
    node end; 
    int size; 
    elem_free data_free;
    print_elem print_data; 
    are_equal data_equals;
};

/**************************/
/* Interface */
/*************************/

/* @brief: construct a new list
 * @param data_free: fn ptr to free an element of given type
 * @param print_data: fn ptr to print an element of given type
 * @param data_equals: fn ptr that checks if two data elements are equal
 * @runtime: O(1) 
 * @return: a valid doubly linked list */
list new_list(elem_free data_free, print_elem print_data, are_equal data_equals);

/* @brief: insert a data element at the front of the list
 * @param data: data to be inserted
 * @param l: list to perform append on 
 * @runtime: O(1) */
 void append_node(elem data, list l);

/* @brief: insert a data element at the end of the list
 * @param data: data to be inserted
 * @param l: list to perform prepend on 
 * @runtime: O(1) */ 
void prepend_node(elem data, list l);

/* @brief: delete all nodes containing matching data
 * @param data: element to be deleted from the list
 * @param: list to perform delete on 
 * @runtime: O(n) */
elem delete_nodes(elem data, list l); 

/* @brief: map a function across all data elements in the list
 * @param l: list to perform map on
 * @param: fn: fn ptr to be invoked on each data element
 * @runtime: O(1) 
 */
void map(list l, map_fn fn); 

/* @brief: return the number of nodes in the list
 * @param l: list to perform count on 
 * @runtime: O(1) */
int list_size(list l);

/* @brief: print the contents of the list 
 * @param l: list to perform print on
 * @runtime: O(n) */
void print_list(list l); 

/* @brief: free the contents of the list
 * @param l: list to free 
 * @runtime: O(n) */
void free_list(list l); 

#endif // LIST_H
