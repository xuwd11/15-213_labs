#include "list.h"
#include "contracts.h"

/*************************/
/* Contracts */
/*************************/

static bool is_linked(list l) {
    REQUIRES(l != NULL && l->start != NULL && l->end != NULL);

    node p = l->start->next;
    while (p != l->end) { 
        if (p == NULL) { printf("NULL"); return false; } 
        if (p->next == NULL) {printf("NULL1"); return false;  } 
        if (p->next->prev != p) return false;
        p = p->next; 
    }
    p = l->end->prev;
    while (p != l->start) { 
        if (p == NULL) return false; 
        if (p->prev == NULL) return false;   
        if (p->prev->next != p) return false; 
        p = p->prev;
    }
    return true; 
}

static int count_nodes(list l) {
    int size = 0;  
    node p = l->start->next; 
    while (p != l->end) {
        size++; 
        p = p->next;
    } 
    return size; 
}

static bool valid_fn_ptrs(list l) { 
    return l->print_data != NULL && l->data_equals != NULL;     
}


static bool is_linked_list(list l) { 
    if (l == NULL) return false; 
    if (l->start == NULL || l->end == NULL) return false; 
    if (!is_linked(l)) return false; 
    if (l->size != count_nodes(l)) return false; 
    if (!valid_fn_ptrs(l)) return false; 
    return true; 
}


/****************************/
/* Implementation Internals */
/****************************/

static void delete_node(node p, list l) { 
    node prev = p->prev; 
    node next = p->next;
    prev->next = next; 
    next->prev = prev;
    
    // free the node/contents
    if (l->data_free) { // use client-function to free
        printf("free data\n");  
        (*(l->data_free))(p->data); 
    }
    free(p);
    l->size--;  
    return; 
} 

/****************************/
/* Interface Implementation */
/****************************/

// just link up the start/end nodes and assign the function 
// pointers will use in the implementation
list new_list(elem_free data_free, print_elem print_data, 
        are_equal data_equals) { 
    REQUIRES(print_data != NULL); 
    REQUIRES(data_equals != NULL); 

    // construct list/start/end nodes
    list l = xmalloc(sizeof(struct linked_list));
    l->start = xmalloc(sizeof(struct list_node)); 
    l->end = xmalloc(sizeof(struct list_node));
    
    // connect nodes
    l->start->next = l->end; 
    l->end->prev = l->start;
    l->size = 0;
    
    // add function ptr's
    l->data_free = data_free; 
    l->print_data = print_data;
    l->data_equals = data_equals;
    
    ENSURES(is_linked_list(l)); 
    return l; 
}

// create a node and then have it point at the start node/
// node after the start node and have the start node/node 
// after the start node point to it
void append_node(elem e, list l) { 
    REQUIRES(e != NULL);
    REQUIRES(is_linked_list(l)); 

    node n = xmalloc(sizeof(struct list_node)); 
    n->data = e; 
    
    // add at end
    n->prev = l->end->prev; 
    n->next = l->end; 
    l->end->prev->next = n; 
    l->end->prev = n; 
    l->size++;

    ENSURES(is_linked_list(l)); 
}

// just like append; create a node and then have it 
// point at the start node/node after the start node
// and have the start node/node after the start node 
// point to it
void prepend_node(elem e, list l) { 
    REQUIRES(e != NULL);
    REQUIRES(is_linked_list(l)); 

    node n = xmalloc(sizeof(struct list_node)); 
    n->data = e; 

    // add at front
    n->prev = l->start; 
    n->next = l->start->next; 
    l->start->next->prev = n; 
    l->start->next = n;
    l->size++; 

    ENSURES(is_linked_list(l)); 
}

// if we find an element that matches the one we're trying to 
// delete, we can immediately delete it from the list.  However, 
// before doing so, get a reference to the next node; we won't be 
// able to proceed in the linked list from the node we're about to free!
elem delete_nodes(elem e, list l) {
    REQUIRES(e != NULL);
    REQUIRES(is_linked_list(l)); 

    elem found_elem = NULL; 
    node p = l->start->next;
    while (p != l->end) {
        // matching element found
        if ((*(l->data_equals))(e, p->data)) { 
            found_elem = p->data;
            // move to next node, delete current node 
            node n = p->next;     
            delete_node(p, l); 
            p = n; 
            continue; 
        } 
        p = p->next;
    }

    ENSURES(is_linked_list(l)); 
    return found_elem; 
}

// we simply iterate over the list and invoke the 
// function on each element by dereferencing the function pointer
void map(list l, map_fn fn) { 
    REQUIRES(is_linked_list(l)); 
    REQUIRES(fn != NULL); 

    node p = l->start->next; 
    while(p != l->end) { 
        (*fn)(p->data);  
        p = p->next;
    } 

    ENSURES(is_linked_list(l));
}

int list_size(list l) { 
    REQUIRES(is_linked_list(l)); 
    return l->size; 
}

// iterate and print.  It's that easy
void print_list(list l) { 
    REQUIRES(is_linked_list(l)); 

    node p = l->start->next; 
    while(p != l->end) { 
        (*(l->print_data))(p->data);  
        p = p->next;
    } 
    printf("\n"); 

    ENSURES(is_linked_list(l));
}

// implementing this is a little tricky.  First, we need 
// to check if we have to free the data at each node (indicated 
// by whether the client provided a function to do so.  If not, too bad, 
// it's not the library's fault.  
// Second, we can't just free a node and carry on to the next one; 
// we need to store a reference to the next node (can't dereference free'd 
// memory!)
void free_list(list l) {
    REQUIRES(is_linked_list(l));

    node p = l->start->next;
    node q; 
    while (p != l->end) {
        // move onto next node, free prev node
        q = p; 
        p = q->next;
        if (l->data_free) { 
            // free using the client-provided function 
            (*(l->data_free))(q->data);
            printf("free\n"); 
        }
        free(q);
    }
    free(l->start);
    free(l->end); 
    free(l); 
 }
