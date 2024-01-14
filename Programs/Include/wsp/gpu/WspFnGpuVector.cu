
#include <stdio.h>
#include <cutil_inline.h>

#include <wsp/gpu/WspFnGpuVector.h>


wsp::State wsp::gpu::VectorAdd(float *o_vec, float *in_vecA, float *in_vecB, int length){
    float* d_A, *d_B, *d_C;

    size_t size = length * sizeof(float);

    // Allocate vectors in device memory
    cutilSafeCall( cudaMalloc((void**)&d_A, size) );
    cutilSafeCall( cudaMalloc((void**)&d_B, size) );
    cutilSafeCall( cudaMalloc((void**)&d_C, size) );

    // Copy vectors from host memory to device memory
    cutilSafeCall( cudaMemcpy(d_A, in_vecA, size, cudaMemcpyHostToDevice) );
    cutilSafeCall( cudaMemcpy(d_B, in_vecB, size, cudaMemcpyHostToDevice) );

    // Invoke kernel
    int threadsPerBlock = 256;
    int blocksPerGrid = (length + threadsPerBlock - 1) / threadsPerBlock;
    wsp::gpu::VectorAdd_d<<<blocksPerGrid, threadsPerBlock>>>(d_A, d_B, d_C, length);
    cutilCheckMsg("kernel launch failure");

#ifdef _DEBUG
    cutilSafeCall( cutilDeviceSynchronize() );
#endif

    // Copy result from device memory to host memory
    cutilSafeCall( cudaMemcpy(o_vec, d_C, size, cudaMemcpyDeviceToHost) );
    
    cudaFree(d_A);
    cudaFree(d_B);
    cudaFree(d_C);
    cutilDeviceReset();

    return WSP_STATE_SUCCESS;
}
