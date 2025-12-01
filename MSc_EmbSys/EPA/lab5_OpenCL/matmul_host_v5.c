#include <stdio.h>
#include <stdlib.h>
#include <math.h> // fabs()
#include <sys/time.h> // struct timeval, gettimeofday()
#include <CL/opencl.h> // OpenCL headers
#include <inttypes.h> // PRIx32

#define M_SIZE (64)
#define VALUES M_SIZE*M_SIZE
#define LOCAL_SIZE (1)
#define LOG_SIZE (2048)

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

int main(int argc, char *argv[]) {
	
	// Local variables
	int i, j,values, errors, m_size, local_size;
	char kernel_name[150];
	// Host memory
	cl_uint *a_host = NULL;
	cl_uint *b_host = NULL;
	cl_uint *c_host = NULL;
	
	cl_uint *c_ref = NULL;
	
	// Device memory
	cl_mem a_device;
	cl_mem b_device;
	cl_mem c_device;
	
	// Performance measurements
	struct timeval t0, tf;
	float ts, tp, tb;
	
	// OpenCL variables
	FILE *f;
	size_t f_size;
	size_t global[3] = {0}, local[3] = {0};
	char *buffer = NULL;
	cl_int ret;
	cl_platform_id platform;
	cl_device_id device;
	cl_context context;
	cl_command_queue queue;
	cl_program program;
	cl_kernel kernel;
	
	/* [1] Initialize application */
	
	// Read command line arguments to configure run
	m_size = (argc > 1) ? atoi(argv[1]) : M_SIZE;
	local_size = (argc > 1) ? atoi(argv[2]) : LOCAL_SIZE;
	values = m_size * m_size;
	
	//Selecting kernel depending on the block size (local work group size), kernel do not acept variable size vectors
	switch (local_size){
		case 1:
			strcpy(kernel_name,"matmul_kernel_BS1.cl");
			break;
		case 2:
			strcpy(kernel_name,"matmul_kernel_BS2.cl");
			break;
		case 4:
			strcpy(kernel_name,"matmul_kernel_BS4.cl");
			break;
		case 8:
			strcpy(kernel_name,"matmul_kernel_BS8.cl");
			break;
		case 16:
			strcpy(kernel_name,"matmul_kernel_BS16.cl");
			break;
		default:
			strcpy(kernel_name,"matmul_kernel_BS4.cl");
			break;
	}
	
	printf("-------------Matrix block based multiplication with OpenCL (Matrix size %d x %d)------------------\n", m_size,m_size);
	printf("\n");
	printf("+++++++++++Work groups size = %d +++++++++++++++++\n", local_size);
	printf("Matrix blocks size is %d x %d\n", local_size, local_size);
	printf("\n");
	
	// Allocate memory for host variables
	a_host = malloc(values * sizeof *a_host);
	b_host = malloc(values * sizeof *b_host);
	c_host = malloc(values * sizeof *c_host);
	c_ref = malloc(values * sizeof *c_ref);

	
	// Initialize input arrays
	for (i = 0; i < values; i++) {
		a_host[i] = rand();
		b_host[i] = rand();
	}
	
	/* [2] Initialize OpenCL environment */
	// Get platform
	ret = clGetPlatformIDs(1, &platform, NULL);
	
	// Get device
	ret = clGetDeviceIDs(platform, CL_DEVICE_TYPE_ALL, 1, &device, NULL);// Create context--> system dependent
	context = clCreateContext(0, 1, &device, NULL, NULL, &ret);
	
	// Create command queue
	queue = clCreateCommandQueue(context, device, 0, &ret);
	
	/* [3] Compile OpenCL kernel */
	// Get kernel file size (kernel.cl)
	f = fopen(kernel_name, "rb");
	fseek(f, 0, SEEK_END);
	f_size = ftell(f);
	rewind(f);
	
	// Read file into memory
	buffer = malloc(f_size + 1);
	buffer[f_size] = '\0';
	fread(buffer, sizeof (char), f_size, f);
	fclose(f);
	
	// Create program
	printf("<OpenCL> Kernel(%s) source:\n", kernel_name);
	printf("%s", buffer);
	program = clCreateProgramWithSource(context, 1, (const char **)&buffer, &f_size, &ret);
	
	// Build (i.e., compile) program
	printf("<OpenCL> Building kernel...\n");
	gettimeofday(&t0, NULL);
	ret = clBuildProgram(program, 0, NULL, "-cl-std=CL1.2", NULL, NULL);
	gettimeofday(&tf, NULL);
	tb = ((tf.tv_sec - t0.tv_sec) * 1000.0) + ((tf.tv_usec - t0.tv_usec) / 1000.0);
	printf("Build time : %.3f ms\n", tb);
	
	// Print build log (optional)
	char log[LOG_SIZE];
	ret = clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, LOG_SIZE, log, NULL);
	printf("<OpenCL> Kernel build log:\n%s\n", log);
	
	/* [4] Configure OpenCL kernel */
	// Create kernel
	kernel = clCreateKernel(program, "matmul", &ret);
	
	// Create device buffers
	a_device = clCreateBuffer(context, CL_MEM_READ_ONLY,values * sizeof *a_host, NULL, &ret);
	b_device = clCreateBuffer(context, CL_MEM_READ_ONLY, values * sizeof *b_host, NULL, &ret);
	c_device = clCreateBuffer(context, CL_MEM_WRITE_ONLY, values * sizeof *c_host, NULL, &ret);
	
	// Set kernel parameters
	ret = clSetKernelArg(kernel, 0, sizeof (cl_mem), &a_device);
	ret |= clSetKernelArg(kernel, 1, sizeof (cl_mem), &b_device);
	ret |= clSetKernelArg(kernel, 2, sizeof (cl_mem), &c_device);
	
	/* [5] Execute kernel */
	printf("<OpenCL> Executing kernel...\n");
	gettimeofday(&t0, NULL);
	
	// Write data from host to device
	ret = clEnqueueWriteBuffer(queue, a_device, CL_TRUE, 0, values * sizeof *a_host, a_host, 0, NULL, NULL);
	ret |= clEnqueueWriteBuffer(queue, b_device, CL_TRUE, 0, values * sizeof *b_host, b_host, 0, NULL, NULL);
	
	// Enqueue kernel for execution
	global[0] = m_size;
	global[1] = m_size;
	local[0] = local_size;
	local[1] = local_size;
	
	
	ret = clEnqueueNDRangeKernel(queue, kernel, 2, NULL, global, local, 0, NULL, NULL);
	
	// Wait for kernel to finish
	ret = clFinish(queue);
	
	// Read data from device to host
	ret = clEnqueueReadBuffer(queue, c_device, CL_TRUE, 0, values * sizeof *c_host, c_host, 0, NULL, NULL);
	
	gettimeofday(&tf, NULL);
	tp = ((tf.tv_sec - t0.tv_sec) * 1000.0)	+ ((tf.tv_usec - t0.tv_usec) / 1000.0);
	printf("[PAR] Execution time : %.3f ms\n", tp);
	
	/* [6] Print results, perform checks */
	// Compute golden reference and check errors
	gettimeofday(&t0, NULL);
	
	errors = 0;
	
	matmul_ref(m_size, a_host, b_host, c_ref);
	
	for (i = 0; i < values; i++) {
		if (c_ref[i] != c_host[i]){
			errors++;
		} 	
	}
	
	
	gettimeofday(&tf, NULL);
	ts = ((tf.tv_sec - t0.tv_sec) * 1000.0) + ((tf.tv_usec - t0.tv_usec) / 1000.0);
	printf("[SEQ] Execution time : %.3f ms\n", ts);
	printf("Found %d error%s\n", errors, (errors == 1) ? "" : "s");
	printf("\n");
	
	// Matrix rpresentation
	printf("Matrix A @ %p\n", a_host);
    for (i = 0; i < 4; i++) {
        printf("    ");
        for (j = 0; j < 4; j++) {
            printf("%08" PRIx32 " ", a_host[i*m_size + j]);
        }
        printf("\n");
    }
    printf("Matrix B @ %p\n", b_host);
    for (i = 0; i < 4; i++) {
        printf("    ");
        for (j = 0; j < 4; j++) {
            printf("%08" PRIx32 " ", b_host[i*m_size + j]);
        }
        printf("\n");
    }
    printf("Matrix C @ %p\n", c_host);
    for (i = 0; i < 4; i++) {
        printf("    ");
        for (j = 0; j < 4; j++) {
            printf("%08" PRIx32 " ", c_host[i*m_size + j]);
        }
        printf("\n");
    }
    
    printf("Matrix Cref @ %p\n", c_ref);
    for (i = 0; i < 4; i++) {
        printf("    ");
        for (j = 0; j < 4; j++) {
            printf("%08" PRIx32 " ", c_ref[i*m_size + j]);
        }
        printf("\n");
    }
	
	
	/* [7] Cleanup system */
	// Cleanup host variables
	free(a_host);
	free(b_host);
	free(c_host);
	free(c_ref);
	free(buffer);
	
	// Cleanup OpenCL
	clReleaseMemObject(a_device);
	clReleaseMemObject(b_device);
	clReleaseMemObject(c_device);
	clReleaseKernel(kernel);
	clReleaseProgram(program);
	clReleaseCommandQueue(queue);
	clReleaseContext(context);
	return 0;
}
