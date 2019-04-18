/* 
 * trans.c - Matrix transpose B = A^T
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 */ 
#include <stdio.h>
#include "cachelab.h"

int is_transpose(int M, int N, int A[N][M], int B[M][N]);

/* 
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded. 
 */
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{   
    if (M == 32 && N == 32) {
        int i, j, i1, j1, d, tmp, b = 8;

        for (i = 0; i < N; i += b)
            for (j = 0; j < M; j += b)
                for (i1 = i; i1 < i + b && i1 < N; i1++) {
                    for (j1 = j; j1 < j + b && j1 < M; j1++) {
                        if (i1 != j1)
                            B[j1][i1] = A[i1][j1];
                        else {
                            d = i1;
                            tmp = A[d][d];
                        }
                    }
                    if (i == j)
                        B[d][d] = tmp;
                } 
    }

    else if (M == 64 && N == 64) {
        int i, j, i1, j1, t1, t2, t3, t4, t5, t6, t7, t8; 

        for (i = 0; i < N; i += 8)
            for (j = 0; j < M; j += 8) {
                for (i1 = i; i1 < i + 4; i1++) {
                    t1 = A[i1][j];
                    t2 = A[i1][j + 1];
                    t3 = A[i1][j + 2];
                    t4 = A[i1][j + 3];
                    t5 = A[i1][j + 4];
                    t6 = A[i1][j + 5];
                    t7 = A[i1][j + 6];
                    t8 = A[i1][j + 7];                    

                    B[j][i1] = t1;
                    B[j + 1][i1] = t2;
                    B[j + 2][i1] = t3;
                    B[j + 3][i1] = t4;

                    B[j][i1 + 4] = t5;
                    B[j + 1][i1 + 4] = t6;
                    B[j + 2][i1 + 4] = t7;
                    B[j + 3][i1 + 4] = t8;
                }
                
                for (j1 = j + 4; j1 < j + 8; j1++) {
                    t5 = A[i + 4][j1 - 4];
                    t6 = A[i + 5][j1 - 4];
                    t7 = A[i + 6][j1 - 4];
                    t8 = A[i + 7][j1 - 4];

                    t1 = B[j1 - 4][i + 4];
                    t2 = B[j1 - 4][i + 5];
                    t3 = B[j1 - 4][i + 6];
                    t4 = B[j1 - 4][i + 7];

                    B[j1 - 4][i + 4] = t5;
                    B[j1 - 4][i + 5] = t6;
                    B[j1 - 4][i + 6] = t7;
                    B[j1 - 4][i + 7] = t8;

                    B[j1][i] = t1;
                    B[j1][i + 1] = t2;
                    B[j1][i + 2] = t3;
                    B[j1][i + 3] = t4;

                    B[j1][i + 4] = A[i + 4][j1];
                    B[j1][i + 5] = A[i + 5][j1];
                    B[j1][i + 6] = A[i + 6][j1];
                    B[j1][i + 7] = A[i + 7][j1];
                }
            }                
    }

    else {
        int i, j, i1, j1, d, tmp, b = 16;

        for (i = 0; i < N; i += b)
            for (j = 0; j < M; j += b)
                for (i1 = i; i1 < i + b && i1 < N; i1++) {
                    for (j1 = j; j1 < j + b && j1 < M; j1++) {
                        if (i1 != j1)
                            B[j1][i1] = A[i1][j1];
                        else {
                            d = i1;
                            tmp = A[d][d];
                        }
                    }
                    if (i == j)
                        B[d][d] = tmp;
                } 
    }

}

/* 
 * You can define additional transpose functions below. We've defined
 * a simple one below to help you get started. 
 */ 

/* 
 * trans - A simple baseline transpose function, not optimized for the cache.
 */

char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, tmp;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }    

}

/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions()
{
    /* Register your solution function */
    registerTransFunction(transpose_submit, transpose_submit_desc); 

    /* Register any additional transpose functions */
    //registerTransFunction(trans, trans_desc); 

}

/* 
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; ++j) {
            if (A[i][j] != B[j][i]) {
                return 0;
            }
        }
    }
    return 1;
}

