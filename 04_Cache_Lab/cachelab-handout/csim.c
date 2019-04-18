#include "cachelab.h"
#include <getopt.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

enum {
    HIT,
    MISS,
    EVICTION
};

typedef struct {
    int verbose, help, s, S, E, b;
} cache_params;

typedef struct {
    int valid;
    unsigned long long tag;
} cache_line;

typedef struct {
    char operation;
    unsigned long long tag, set_index;
    int size;
} operation;

typedef struct {
    int hit_count, miss_count, eviction_count;
} summary;

void print_help_info(void) {
    ;
}

int main(int argc, char** argv) {
    cache_params params;
    params.verbose = 0;
    char* tracefile;
    int opt, trace = 0;
    while(-1 != (opt = getopt(argc, argv, "vhs:E:b:t:"))) {
        switch (opt) {
            case 'v':
                params.verbose = 1;
                break;
            case 'h':
                params.help = 1;
                break;
            case 's':
                params.s = atoi(optarg);
                params.S = 1 << params.s;
                break;
            case 'E':
                params.E = atoi(optarg);
                break;
            case 'b':
                params.b = atoi(optarg);
                break;
            case 't':
                trace = 1;
                tracefile = optarg;
                break;
            default:
                printf("Argument error!\n\n");
                print_help_info();
                exit(1);
        }
    }

    if (params.s <= 0 || params.E <= 0 || params.b <= 0 || trace == 0) {
        printf("Argument error!\n\n");
        print_help_info();
        exit(1);
    }

    if (params.help == 1) {
        print_help_info();
        exit(0);
    }

    FILE* file = fopen(tracefile, "r");
    if (file != NULL);
    fclose(file);

    printSummary(0, 0, 0);
    return 0;
}
