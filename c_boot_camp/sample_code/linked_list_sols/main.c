#include "list.h"
#include "int_fn_ptrs.h"
#include <stdlib.h> 
#include <stdio.h> 
#include <getopt.h>
#include <unistd.h>
#include <errno.h>
#include <stdbool.h> 

list l = NULL; 

void print_illegal_line(int line_num) { 
    printf("Illegal arguments on line %d\n", line_num);  
}

void print_usage(char **argv) { 
    printf("Usage: [-h] [-v] %s -f <trace_file> \n", argv[0]); 
    printf("-h: help\n"); 
    printf("-v: verbose mode\n"); 
    printf("-f: trace file location\n");    
}

void safe_parse_int(char* str, int* d, int line_num) {
    char* end; 
    // better than atoi; look up the documentation online!
    *d = strtol(str, &end, 10); 
    if (*end) { // error occured in parsing
        print_illegal_line(line_num); 
        free_list(l);  
        exit(1);  
    }
}

void print_valid_commands() { 
    fprintf(stderr, "Valid commands: \n"); 
    fprintf(stderr, "append:    a <num>\n"); 
    fprintf(stderr, "prepend:   p <num>\n"); 
    fprintf(stderr, "delete:    d <num>\n"); 
    fprintf(stderr, "map:  m fn\n"); 
    fprintf(stderr, "valid map functions: add_1, square, next_pow_2\n"); 
}

void apply_map_fn(char* map_fn) { 
    if (!strcmp(map_fn, "add_1")) { 
        map(l, &add_1); 
    } else if (!strcmp(map_fn, "square")) { 
        map(l, &square); 
    } else if (!strcmp(map_fn, "next_pow_2")) { 
        map(l, &next_pow_2); 
    } else { 
        print_valid_commands();
        free_list(l); 
        exit(1); 
    }
}

void run_trace(char* file_name, bool verbose) { 
    FILE* file ;
    int read = 0; 
    char cmd; 
    char arg[10];  
    int line_num = 0; 
    if (!(file = fopen(file_name, "r"))) {
        fprintf(stderr, "Could not find file %s\n", file_name); 
        exit(1); 
    } 
    l = new_list(&free, &print_int, &ints_equal);
    
    while ((read = fscanf(file, "%c %s\n", &cmd, arg)) != EOF) {
        if (read != 2) { // failed to match
            fprintf(stderr, "Invalid file format in file %s\n", file_name); 
            free_list(l); 
            exit(1); 
        }
        // IMPORTANT: the linked list code uses 'shallow copies' of the data inserted 
        // into it.  Meaning, that the values stored in the linked list have the same 
        // addresses as the values outside of it.  Since num is used in numerous 
        // iterations of the loop, we therefore need to malloc it to avoid 
        // corrupting the data in the linked list (passing the address of num 
        // won't work, since the value of num is changing and all the nodes in the 
        // list would contain the same address of num) 
        int* num = malloc(sizeof(int)); // used if the cmd uses a number 
        if (verbose) { 
            printf("%c %s: ", cmd, arg);
        }    
        switch(cmd) {
            case 'a': // append
                safe_parse_int(arg, num, line_num);
                append_node(num, l); 
                break; 
            case 'p': // prepend
                safe_parse_int(arg, num, line_num);
                prepend_node(num, l);
                break; 
            case 'd': // delete
                safe_parse_int(arg, num, line_num);
                delete_nodes(num, l);
                free(num); 
                break; 
            case 'm': // map
                apply_map_fn(arg);
                free(num); 
                break;
            default: // illegal cmd
                print_valid_commands();
                free_list(l); 
                exit(1); 
        }
        if (verbose) { 
            print_list(l); 
        }
        line_num++; 
    }
    printf("Final list: \n"); 
    print_list(l); 
    free_list(l);
    fclose(file);  
} 

int main(int argc, char **argv) {
    char c; 
    char* file_name = NULL;  
    bool verbose = false; 
    while((c = getopt(argc, argv, "hvf:")) != -1) { 
        switch(c) { 
            case 'h': 
                print_usage(argv); 
                exit(0); 
            case 'v': 
                verbose = true; 
                break;
            case 'f': 
                file_name = optarg; 
                break; 
            default: 
                print_usage(argv);
                exit(1);  
        }    
    }
    if (file_name == NULL) { 
        print_usage(argv); 
        exit(1); 
    }
    run_trace(file_name, verbose); 
}
