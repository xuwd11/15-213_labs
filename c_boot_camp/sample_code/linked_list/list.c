#include "list.h"

/*************************/
/* Contracts */
/*************************/

bool is_linked(list l) {
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

int count_nodes(list l) {
    int size = 0;  
    node p = l->start->next; 
    while (p != l->end) {
        size++; 
        p = p->next;
    } 
    return size; 
}

bool is_linked_list(list l) { 
    if (l == NULL) return false; 
    if (l->start == NULL || l->end == NULL) return false; 
    if (!is_linked(l)) return false; 
    if (l->size != count_nodes(l)) return false; 
    return true; 
}

/*************************/
/* Implementation */
/*************************/
list new_list(elem_free data_free, print_elem print_data, are_equal data_equals) { 
    
    /* TODO: Finish this function */
    list l = xmalloc(sizeof(struct linked_list));
    l->size = 0;
    // so gcc doesn't yell at us
    (void)l;
    (void)data_free; 
    (void)print_data; 
    (void)data_equals;   
    ENSURES(is_linked_list(l)); 
    return l; 
}

void append_node(elem e, list l) { 
    REQUIRES(e != NULL);
    REQUIRES(is_linked_list(l)); 

    /* TODO: Finish this function */
    node n = xmalloc(sizeof(struct list_node)); 
    n->data = e; 

    (void)n; 
    ENSURES(is_linked_list(l)); 

}

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

elem delete_nodes(elem e, list l) {
    REQUIRES(e != NULL);
    REQUIRES(is_linked_list(l)); 

    /* TODO: Finish this function */

    ENSURES(is_linked_list(l)); 
    return NULL;  
}

void map(list l, map_fn fn) { 
    REQUIRES(is_linked_list(l)); 

    /* TODO: Finish this function */
    (void)fn; 

    ENSURES(is_linked_list(l));
}

int list_size(list l) { 
    REQUIRES(is_linked_list(l)); 
    return l->size; 
}

void print_list(list l) { 
    REQUIRES(is_linked_list(l)); 

    /* TODO: Finish this function */

    ENSURES(is_linked_list(l));

}

void free_list(list l) {
    REQUIRES(is_linked_list(l));

    /* TODO: Finish this function */
}
