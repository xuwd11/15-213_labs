#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "contracts.h"
#include "types.h"

#ifndef INT_FN_PTRS_H
#define INT_FN_PTRS_H

/* @brief: prints the contents of a ptr to an int
 * @param e: address of memory block whose contents 
 * are to be printed */
void print_int(elem e); 

/* @brief: compares the contents of two ptrs to 
 * ints; indicates if they're equal or not
 * @param e1: address of element 1 to be compared
 * @param e2: address element 2 to be compared 
 * @return: true if equal, else false */
bool ints_equal(elem e1, elem e2); 

/* @brief: adds 1 to the value at address stored in ptr 
 * @param e: address of value */
void add_1(elem e); 

/* @brief: squares the value at address stored in ptr
 * @param e: address of value */
void square(elem e); 

/* @brief: finds the next power of 2 from the value 
 * at address stored in ptr
 * @param e: address of value */ 
void next_pow_2(elem e);

#endif // INT_FN_PTRS_H
