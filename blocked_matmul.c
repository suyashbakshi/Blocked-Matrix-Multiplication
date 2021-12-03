#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <time.h>
#include <omp.h>

int BLOCK_SIZE[] = {32,64,128,256,512};
int THREADS[] = {4,16,24,48,96};

#define MATRIX_SIZE 2048

double A[MATRIX_SIZE][MATRIX_SIZE],
B[MATRIX_SIZE][MATRIX_SIZE],
C[MATRIX_SIZE][MATRIX_SIZE];

int min(int a, int b)
{
	return a < b ? a : b;
}

int main(int argc, char*  argv[])
{
	// Initalize our matrix looping variables once
	int k, j, i, jj, kk, bb, th, block_idx = 5, thread_idx = 5;
	
	// Initalize array A and B with random variables
	for (i = 0; i < MATRIX_SIZE; ++i) {
		for (k = 0; k < MATRIX_SIZE; ++k) {
			A[i][k] = rand() % (101);
			B[i][k] = rand() % (101);
		}
	}
	// Run for each block size
	for (bb = 0; bb < block_idx; ++bb)
	{
		printf("BLOCK:%d\n", BLOCK_SIZE[bb]);
		
		// Iterate through different number of threads
		
		for (th=0; th < thread_idx; th++)
		{
			memset(C, 0, sizeof(C[0][0] * MATRIX_SIZE * MATRIX_SIZE));
			printf("Threads:%d\n",THREADS[th]);

			omp_set_num_threads(THREADS[th]);
		
			double start = omp_get_wtime();	
			// Do block matrix multiplication
	
			//BLOCKING FOR L1 memory
			//Loop order => kk, jj, ii == Block outer product, with horizontal-block traversal of 'B'
			//Loop order => jj, kk, ii == Block outer product, with vertical-block traversal of 'B'
			//Loop order => ii, jj, kk == Block inner product, with full computation of one 'C' block in innermost loop
			//							and computation of all horizontal-blocks of 'C' in middle loop
			//Loop order => ii, kk, jj == Block inner product, with partial computations of 'C' horizontal-blocks in innermost loop,
			//							and full computation of all those 'C' horizontal-blocks in middle loop
			//Loop order => jj, ii, kk == Block inner product, with full computation of one 'C' block in innermost loop,
			//							and computation of all vertical-blocks of 'C' in middle loop
			//Loop order => kk, ii, jj == Block inner product, with partial computations of 'C' horizontal-blocks in innermost loop,
			//							partial computations of next row of horizontal-blocks of 'C' in middle loop,
			//							followed by full computation of horizontal-blocks of 'C' from 1st row in outermost loop
			for (kk = 0; kk < MATRIX_SIZE; kk += BLOCK_SIZE[bb]) {
				for (jj = 0; jj < MATRIX_SIZE; jj += BLOCK_SIZE[bb]) {
					for (ii = 0; ii < MATRIX_SIZE; ii += BLOCK_SIZE[bb]) {
				
						//BLOCKING FOR L0 memory
						for (i = ii; i < min(ii + BLOCK_SIZE[bb], MATRIX_SIZE); ++i) {
							for (j = jj; j < min(jj + BLOCK_SIZE[bb], MATRIX_SIZE); ++j){
								for (k = kk; k < min(kk + BLOCK_SIZE[bb], MATRIX_SIZE); ++k) {
									
									C[i][j] += A[i][k] * B[k][j];
								}
							}
						}
					}
				}
			}

			// Keep track of when we finish our work
    		double end = omp_get_wtime();
    		printf("Time:%1.9f\n",end-start);
			
		}
		
		printf("\n");
	}
	
	return 0;
}

