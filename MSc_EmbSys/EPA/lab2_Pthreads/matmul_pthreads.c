#include <stdio.h>
#include <stdlib.h>
#include <stdint.h> // uint32_t
#include <inttypes.h> // PRIx32
#include <sys/time.h> // struct timeval, gettimeofday()
#include <pthread.h>


struct arg_struct { 
    int id;
    int threads_number;
    size_t size;
    uint32_t *a;
    uint32_t *b;
    uint32_t *c;
};

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

// Thread operation -->Divide matrix rows bt the number of threads
void *thread(void * arguments) {
	
	struct arg_struct *args = arguments;
	int i, j, k;
	int id = args -> id;
	
	size_t size = args -> size;
	int threads_number = args -> threads_number;
	uint32_t *a = args -> a;
	uint32_t *b = args -> b;
	uint32_t *c = args -> c;
	int offset = id*size/threads_number;
	
	for (i = 0 + offset; i < size / threads_number + offset ; i++) {
		for (j = 0; j < size; j++) {
			c[i*size + j] = 0;
			for (k = 0; k < size; k++) {
				c[i*size + j] += a[i*size + k] * b[k*size + j];
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
	
	//Pthreads 
	pthread_t threads[THREADS];
		
	struct arg_struct  *args = malloc(THREADS * sizeof *args); //struct with the arguments for pthreads function
	
	
	
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
	
	// Preads implementation -----------------------
	//Load pthreads arguments
	for(i = 0; i < THREADS; i ++){
		
		args[i].id = i;
		args[i].size = size;
		args[i].threads_number = THREADS;
		args[i].a = a;
		args[i].b = b;
		args[i].c = caux;
		
		
	}
	
	// Pthreads operation
	gettimeofday(&t0, NULL);
	for(i = 0; i < THREADS; i ++){
		pthread_create(&threads[i], NULL, thread, &args[i]);
	}
	for (i = 0; i < THREADS; i ++){
		pthread_join(threads[i], NULL);
	}
	gettimeofday(&tf, NULL);
	
	t = ((tf.tv_sec - t0.tv_sec) * 1000.0) + ((tf.tv_usec - t0.tv_usec) / 1000.0);
	printf("Execution time (pthreads implementation): %.3f ms\n", t);
	
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
	free(args);
	
	return 0;
}
