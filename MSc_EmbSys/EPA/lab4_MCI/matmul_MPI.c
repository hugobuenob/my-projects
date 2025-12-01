#include <mpi.h> // MPI headers
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h> // uint32_t
#include <inttypes.h> // PRIx32



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

// Parallel operation -->Divide matrix rows bt the number of ranks
void matmul_MPI(int matrix_size, int size, uint32_t *a, uint32_t *b, uint32_t *c) {
	
	
	int i, j, k;
	for (i = 0; i < matrix_size / size; i++) {
		for (j = 0; j < matrix_size; j++) {
			c[i*matrix_size + j] = 0;
			for (k = 0; k < matrix_size; k++) {
				c[i*matrix_size + j] += a[i*matrix_size + k] * b[k*matrix_size + j];		
			}
		}
	}
	return NULL;
	
}


// Main program
int main(int argc, char *argv[]) {

	
	int i, j;
	uint32_t *a_global = NULL, *b_global = NULL, *c_global = NULL, *caux_global = NULL;
	uint32_t *a_local = NULL, *caux_local = NULL;
	struct timeval t0, tf;
	float t;
	size_t matrix_size, max_values;
	
	matrix_size = (argc > 1) ? ((atoi(argv[1]) > 0)? atoi(argv[1]) : 1024) : 1024;
	max_values = matrix_size * matrix_size;
	
	// Initialize the MPI environment
	MPI_Init(NULL, NULL);
	
	// Get the number of processes
	int world_size;
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);
	
	// Get the rank of the process
	int world_rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
	
	int block_size = matrix_size/world_size;
	
	// Allocate global memory buffer
	a_global = malloc(matrix_size * matrix_size * sizeof *a_global);
	b_global = malloc(matrix_size * matrix_size * sizeof *b_global);
	c_global = malloc(matrix_size * matrix_size * sizeof *c_global);
	caux_global = malloc(matrix_size * matrix_size * sizeof *caux_global);
	
	if (world_rank == 0){
		printf("Matrix multiplication (%zux%zu) ----------------------------------------------------\n", matrix_size, matrix_size);
		printf("Number of ranks %d\n", world_size);
		// Initialize matrix
		for (i = 0; i < matrix_size; i++) {
			for (j = 0; j < matrix_size; j++) {
				a_global[i*matrix_size + j] = rand();
				b_global[i*matrix_size + j] = rand();
			}
		}
		// Naive implementation ------------------------------------------------------------------------
		
		gettimeofday(&t0, NULL);
		matmul_ref(matrix_size, a_global, b_global, c_global);
		gettimeofday(&tf, NULL);
		t = ((tf.tv_sec - t0.tv_sec) * 1000.0) + ((tf.tv_usec - t0.tv_usec) / 1000.0);
		printf("Execution time (naive implementation) : %.3f ms\n", t);	
		
		
	}
	
	//Allocate local buffers
	a_local = malloc(block_size * matrix_size * sizeof *a_local);
	caux_local = malloc(block_size * matrix_size * sizeof *caux_local);
	
	//Send data
	MPI_Bcast(b_global, max_values, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Scatter(a_global, block_size*matrix_size, MPI_INT, a_local, block_size*matrix_size, MPI_INT, 0, MPI_COMM_WORLD);
	
	
	//MPI parallel implementation --------------------------------------------------------------------------
	MPI_Barrier(MPI_COMM_WORLD);
	if (world_rank == 0){
		gettimeofday(&t0, NULL);
	}
	matmul_MPI(matrix_size, world_size,  a_local, b_global, caux_local);
	
	// Receive data from slave processes
	MPI_Barrier(MPI_COMM_WORLD);
	MPI_Gather(caux_local, block_size * matrix_size, MPI_INT, caux_global, block_size * matrix_size, MPI_INT, 0, MPI_COMM_WORLD);
	
	if (world_rank == 0){
		gettimeofday(&tf, NULL);
		t = ((tf.tv_sec - t0.tv_sec) * 1000.0) + ((tf.tv_usec - t0.tv_usec) / 1000.0);
		printf("Execution time (MPI implementation): %.3f ms\n", t);
		
		//Matrix representation
		
		printf("Matrix A @ %p\n", a_global);
		
		for (i = 0; i < 8; i++) {
			printf(" ");
			for (j = 0; j < 8; j++) {
				printf("%08" PRIx32 " ", a_global[i*matrix_size + j]);
			}
			printf("\n");
		}
		
		printf("Matrix B @ %p\n", b_global);
		for (i = 0; i < 8; i++) {
			printf(" ");
			for (j = 0; j < 8; j++) {
				printf("%08" PRIx32 " ", b_global[i*matrix_size + j]);
			}
			printf("\n");
		}
		printf("Matrix C @ %p\n", c_global);
		for (i = 0; i < 8; i++) {
			printf(" ");
			for (j = 0; j < 8; j++) {
				printf("%08" PRIx32 " ", c_global[i*matrix_size + j]);
			}
			printf("\n");
		}
		printf("Matrix Caux @ %p\n", caux_global);
			for (i = 0; i < 8; i++) {
			printf(" ");
				for (j = 0; j < 8; j++) {
				printf("%08" PRIx32 " ", caux_global[i*matrix_size + j]);
				}
			printf("\n");
		}
		
		
	}
	
	
	
	
	// Free local memory buffer
	free(a_global);
	free(b_global);
	free(c_global);
	free(a_local);
	free(caux_local);
	free(caux_global);
	
	// Finalize the MPI environment
	MPI_Finalize();
	return 0;
}
