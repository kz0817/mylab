#include <stdio.h>

__global__ void add(float *arr1, float *arr2, float *ret)
{
    const int tid = blockIdx.x * blockDim.x + threadIdx.x;
    ret[tid] = arr1[tid] + arr2[tid];
}

static void initData(float *arr, const size_t num, const float k)
{
    for (size_t i = 0; i < num; i++)
        arr[i] = k * i + 1;
}

int main (void)
{
    float *arr1, *arr2, *ret;
    float *devArr1, *devArr2, *devRet;

    const size_t numPoints = 8;
    const size_t arrBytes = numPoints * sizeof(float);
    arr1 = (float *)malloc(arrBytes);
    arr2 = (float *)malloc(arrBytes);
    ret  = (float *)malloc(arrBytes);

    initData(arr1, numPoints, 2.1);
    initData(arr2, numPoints, 3.2);

    cudaMalloc((void**)&devArr1, arrBytes);
    cudaMalloc((void**)&devArr2, arrBytes);
    cudaMalloc((void**)&devRet,  arrBytes);

    cudaMemcpy(devArr1, arr1, arrBytes, cudaMemcpyHostToDevice);
    cudaMemcpy(devArr2, arr2, arrBytes, cudaMemcpyHostToDevice);

    const size_t numThreads = 4;
    const size_t numBlocks = 2;
    add<<<numBlocks, numThreads>>>(devArr1, devArr2, devRet);
    cudaMemcpy(ret, devRet, arrBytes, cudaMemcpyDeviceToHost);

    for (size_t i = 0; i < numPoints; i++)
        printf("[%02zd] a: %.3f + b: %.3f => x: %.3f\n", i, arr1[i], arr2[i], ret[i]);

    cudaFree(devArr1);
    cudaFree(devArr2);
    cudaFree(devRet);

    free(ret);
    free(arr2);
    free(arr1);
    return 0;
}
