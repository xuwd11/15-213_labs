# Makefile for Recitation Demo
#
CC = gcc
CFLAGS = -g -Wall -Wextra -Werror -std=c99 
DCFLAGS = -g -Wall -Wextra -Werror -std=c99 -DDEBUG

OBJS = main.o list.o int_fn_ptrs.o xalloc.o

list_demo: $(OBJS) 
	$(CC) $(CFLAGS) -o list_demo $(OBJS)

list_demo_d: $(OBJS) 
	$(CC) $(DCFLAGS) -o list_demo_d $(OBJS) 

xalloc.o: xalloc.c xalloc.h
fnPtrs.o: int_fn_ptrs.c int_fn_ptrs.h
list.o: list.c list.h
main.o: main.c

clean: 
	rm -rf *~ *.o 
