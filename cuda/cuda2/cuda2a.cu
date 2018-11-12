#include <stdio.h>
#include <string>
#include <cassert>
using namespace std;

__global__ void add(float *arr1, float *arr2, float *ret, size_t total)
{
    int tid = blockIdx.x * blockDim.x + threadIdx.x;
    if (tid >= total)
        return;
    ret[tid] = arr1[tid] + arr2[tid];
}

static void initData(float *arr, const size_t num, const float k)
{
    for (size_t i = 0; i < num; i++)
        arr[i] = k * i + 1;
}

struct Args {
    int numBlocks;
    int numThreads;

    struct Ctx {
        int argc;
        char **argv;
        int idx;

        char *get(void)
        {
            return argv[idx];
        }
    };

    int getNextInt(Ctx &ctx)
    {
        assert(ctx.idx != ctx.argc -1);
        ctx.idx++;
        int param = atoi(ctx.get());
        return param;
    }

    Args(int argc, char *argv[])
    : numBlocks(1),
      numThreads(1)
    {
        Ctx ctx {argc, argv, 0};
        for (ctx.idx = 1; ctx.idx < argc; ctx.idx++) {
            const string arg = ctx.get();
            if (arg == "-b") {
                numBlocks = getNextInt(ctx);
            } else if (arg == "-t") {
                numThreads = getNextInt(ctx);
            }
        }
    }
};

int main(int argc, char *argv[])
{
    const Args args(argc, argv);
    printf("numBlocks :  %d\n", args.numBlocks);
    printf("numThreads: %d\n",  args.numThreads);

    float *arr1, *arr2, *ret;
    float *devArr1, *devArr2, *devRet;

    const size_t numPoints = 8;
    const size_t arrBytes = numPoints * sizeof(float);
    arr1 = (float *)malloc(arrBytes);
    arr2 = (float *)malloc(arrBytes);
    ret  = (float *)malloc(arrBytes);

    initData(arr1, numPoints, 2.1);
    initData(arr2, numPoints, 3.2);

    cudaError_t err = cudaSuccess;
    err = cudaMalloc((void**)&devArr1, arrBytes);
    if (err != cudaSuccess) {
        fprintf(stderr, "Failed to allocate devArr1 (error code %s)!\n", cudaGetErrorString(err));
        exit(EXIT_FAILURE);
    }

    err = cudaMalloc((void**)&devArr2, arrBytes);
    if (err != cudaSuccess) {
        fprintf(stderr, "Failed to allocate devArr2 (error code %s)!\n", cudaGetErrorString(err));
        exit(EXIT_FAILURE);
    }

    err = cudaMalloc((void**)&devRet,  arrBytes);
    if (err != cudaSuccess) {
        fprintf(stderr, "Failed to allocate devRet (error code %s)!\n", cudaGetErrorString(err));
        exit(EXIT_FAILURE);
    }

    err = cudaMemcpy(devArr1, arr1, arrBytes, cudaMemcpyHostToDevice);
    if (err != cudaSuccess) {
        fprintf(stderr, "Failed to copy arr1 (error code %s)!\n", cudaGetErrorString(err));
        exit(EXIT_FAILURE);
    }

    err = cudaMemcpy(devArr2, arr2, arrBytes, cudaMemcpyHostToDevice);
    if (err != cudaSuccess) {
        fprintf(stderr, "Failed to copy arr2 (error code %s)!\n", cudaGetErrorString(err));
        exit(EXIT_FAILURE);
    }

    add<<<args.numBlocks, args.numThreads>>>(devArr1, devArr2, devRet, numPoints);
    err = cudaGetLastError();
    if (err != cudaSuccess) {   
        fprintf(stderr, "Failed to launch add kernel (error code %s)!\n", cudaGetErrorString(err));
        exit(EXIT_FAILURE);
    }

    err = cudaMemcpy(ret, devRet, arrBytes, cudaMemcpyDeviceToHost);
    if (err != cudaSuccess) {   
        fprintf(stderr, "Failed to copy devRet (error code %s)!\n", cudaGetErrorString(err));
        exit(EXIT_FAILURE);
    }

    for (size_t i = 0; i < numPoints; i++)
        printf("[%02zd] a: %.3f + b: %.3f => x: %.3f\n", i, arr1[i], arr2[i], ret[i]);

    err = cudaFree(devArr1);
    if (err != cudaSuccess) {   
        fprintf(stderr, "Failed to free devArr1 (error code %s)!\n", cudaGetErrorString(err));
        exit(EXIT_FAILURE);
    }

    err = cudaFree(devArr2);
    if (err != cudaSuccess) {   
        fprintf(stderr, "Failed to free devArr2 (error code %s)!\n", cudaGetErrorString(err));
        exit(EXIT_FAILURE);
    }

    err = cudaFree(devRet);
    if (err != cudaSuccess) {   
        fprintf(stderr, "Failed to free devRet (error code %s)!\n", cudaGetErrorString(err));
        exit(EXIT_FAILURE);
    }

    free(ret);
    free(arr2);
    free(arr1);
    return 0;
}
