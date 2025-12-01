#define BLOCK_SIZE 4

__kernel void matmul(
	__global uint *a,
	__global uint *b,
	__global uint *c			
	)
{
	// Get work size
	size_t gsize = get_global_size(0);
	size_t lsize = get_local_size(0);
	
	// Get work-item IDs
	size_t gidx_row = get_global_id(0);
	size_t gidx_col = get_global_id(1);
	
	size_t lidx_row = get_local_id(0);
	size_t lidx_col = get_local_id(1);
	
	
	size_t block_size = gsize/lsize;
	
	
	// Local variables
	size_t i,j,k,m;
	int sum = 0;
	__local uint a_local[BLOCK_SIZE][BLOCK_SIZE]; //Local matrix to perform the block based multiplication
    __local uint b_local[BLOCK_SIZE][BLOCK_SIZE];
	
	for (int i = 0; i < gsize/BLOCK_SIZE; i++) {
		
        a_local[lidx_row][lidx_col] = a[gidx_row * gsize + (i * BLOCK_SIZE + lidx_col)]; //Each work item loads one value in the local matrix
        b_local[lidx_row][lidx_col] = b[(i * BLOCK_SIZE + lidx_row) * gsize + gidx_col];
        
        barrier(CLK_LOCAL_MEM_FENCE); //Local matrix loaded
       
		
        for (int j = 0; j < BLOCK_SIZE; j++) {
            sum += a_local[lidx_row][j] * b_local[j][lidx_col]; //Block multiplication and acumulation
        }
        
        barrier(CLK_LOCAL_MEM_FENCE); //All work items had finish their operations
    }
    
    
    c[gidx_row *gsize  + gidx_col] = sum; //Once all the blocks have been multiplied each work item loads the result value
   
}
