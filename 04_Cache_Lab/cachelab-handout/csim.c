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
    unsigned long int tag;
} cache_line;

typedef struct {
    char operation;
    unsigned long int address;
    unsigned long int tag, set_index;
    int size;
} operation;

typedef struct {
    int hit_count, miss_count, eviction_count;
} summary;

void print_help_info(void) {
    ;
}

void caching(cache_line* cache, cache_params params, operation op, summary* summ) {
    cache_line* set = cache + op.set_index * params.E;
    int i = 0, result = EVICTION;
    for (; i < params.E; i++) {
        if ((set + i)->valid == 0) {
            result = MISS;
            break;
        }
        if ((set + i)->tag == op.tag) {
            result = HIT;
            break;
        }
    }

    switch (result) {
        case HIT:
            for (; (i < params.E - 1) && ((set + i + 1)->valid == 1); i++)
                (set + i)->tag = (set + i + 1)->tag;
            (set + i)->tag = op.tag;
            summ->hit_count++;
            break;
        case MISS:
            (set + i)->tag = op.tag;
            (set + i)->valid = 1;
            summ->miss_count++;
            break;
        case EVICTION:
            for (i = 0; i < params.E - 1; i++)
                (set + i)->tag = (set + i + 1)->tag;
            (set + i)->tag = op.tag;
            summ->miss_count++;
            summ->eviction_count++;
            break;
    }

    if (op.operation == 'M')
        summ->hit_count++;
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

    cache_line* cache = (cache_line*)malloc(sizeof(cache) * params.S * params.E);
    for (int i = 0; i < params.S * params.E; i++) {
        (cache + i)->valid = 0;
        (cache + i)->tag = 0Xffffffff;
    }

    summary summ;
    summ.hit_count = 0, summ.miss_count = 0, summ.eviction_count = 0;

    operation op;

    FILE* file = fopen(tracefile, "r");
    while (fscanf(file, " %c %lx,%d", &op.operation, &op.address, &op.size) > 0) {
        if (op.operation == 'I')
            continue;
        op.tag = op.address >> (params.s + params.b);
        op.set_index = (op.address >> params.b) & (~(~0 << params.s));
        caching(cache, params, op, &summ);
    }
    fclose(file);
    free(cache);

    printSummary(summ.hit_count, summ.miss_count, summ.eviction_count);
    return 0;
}
