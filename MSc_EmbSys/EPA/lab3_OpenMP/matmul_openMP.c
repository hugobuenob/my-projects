#include <stdio.h>
#include <stdlib.h>
#include <stdint.h> // uint32_t
#include <inttypes.h> // PRIx32
#include <sys/time.h> // struct timeval, gettimeofday()
#include <omp.h>



// Reference implementation: naive algorithm
void matmul_ref(size_t size, uint32_t *a, uint32_t *b, uint32_t *c) {
	
	int i, j, k;
	for (i = 0; i < size; i++) {
		for (j = 0; j < size; j++) {
			c[i*size + j] = 0;
			for (k = 0; k < size; k++) {
				c[i*size + j] += a[i*size + k] * b[k*size + j];
			}
		}
	}
}

// Parallel operation -->Divide matrix rows bt the number of threads
void matmul_omp(int threads_number, size_t size, uint32_t *a, uint32_t *b, uint32_t *c) {
	
	#pragma omp parallel
	{
		int i, j, k;
		int id;
		id = omp_get_thread_num();
		int offset = id*size/threads_number;
		for (i = 0 + offset; i < size / threads_number + offset ; i++) {
			for (j = 0; j < size; j++) {
				c[i*size + j] = 0;
				for (k = 0; k < size; k++) {
					c[i*size + j] += a[i*size + k] * b[k*size + j];
					
				}
			}
		}
	}	
	return NULL;
	
}

// Main program
int main(int argc, char *argv[]) {
	
	int i, j, THREADS;
	size_t size;
	uint32_t *a = NULL, *b = NULL, *c = NULL, *caux = NULL;
	struct timeval t0, tf;
	float t;
	size = (argc > 1) ? ((atoi(argv[1]) > 0)? atoi(argv[1]) : 1024) : 1024;
	THREADS = (argc > 1) ? ((atoi(argv[2]) > 0)? atoi(argv[2]) : 16) : 16;
	
	omp_set_num_threads(THREADS);
	
	
	//System operation 
	printf("Matrix multiplication (%zux%zu)\n", size, size);
	printf("Number of threads %d\n", THREADS);
	a = malloc(size * size * sizeof *a);
	b = malloc(size * size * sizeof *b);
	c = malloc(size * size * sizeof *c);
	caux = malloc(size * size * sizeof *caux);
	
	
	for (i = 0; i < size; i++) {
		for (j = 0; j < size; j++) {
			a[i*size + j] = rand();
			b[i*size + j] = rand();
		}
	}
	
	// Naive implementation -----------------------
	gettimeofday(&t0, NULL);
	matmul_ref(size, a, b, c);
	gettimeofday(&tf, NULL);
	t = ((tf.tv_sec - t0.tv_sec) * 1000.0) + ((tf.tv_usec - t0.tv_usec) / 1000.0);
	printf("Execution time (naive implementation) : %.3f ms\n", t);
	
	//OpenMP parallel implementation -----------------------

	gettimeofday(&t0, NULL);	
	matmul_omp(THREADS, size, a, b, caux);
	gettimeofday(&tf, NULL);
	
	t = ((tf.tv_sec - t0.tv_sec) * 1000.0) + ((tf.tv_usec - t0.tv_usec) / 1000.0);
	printf("Execution time (openMP implementation): %.3f ms\n", t);
	
	//Matrix representation
	printf("Matrix A @ %p\n", a);
	
	for (i = 0; i < 8; i++) {
		printf(" ");
		for (j = 0; j < 8; j++) {
			printf("%08" PRIx32 " ", a[i*size + j]);
		}
		printf("\n");
	}
	
	printf("Matrix B @ %p\n", b);
	for (i = 0; i < 8; i++) {
		printf(" ");
		for (j = 0; j < 8; j++) {
			printf("%08" PRIx32 " ", b[i*size + j]);
		}
		printf("\n");
	}
	printf("Matrix C @ %p\n", c);
	for (i = 0; i < 8; i++) {
		printf(" ");
		for (j = 0; j < 8; j++) {
			printf("%08" PRIx32 " ", c[i*size + j]);
		}
		printf("\n");
	}
	
	printf("Matrix Caux @ %p\n", caux);
	for (i = 0; i < 8; i++) {
		printf(" ");
		for (j = 0; j < 8; j++) {
			printf("%08" PRIx32 " ", caux[i*size + j]);
		}
		printf("\n");
	}
	
	free(a);
	free(b);
	free(c);
	free(caux);
	
	return 0;
}
