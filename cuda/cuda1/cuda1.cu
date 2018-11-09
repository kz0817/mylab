#include <stdio.h>

__global__ void add(int a, int b, int *c)
{
    *c = a + b;
}

int main (void)
{
    int a = 5;
    int b = 3;
    int c;
    int *x = 0;

    cudaMalloc((void**)&x, sizeof(int));
    add<<<1,1>>>(a, b, x);
    cudaMemcpy(&c, x, sizeof(int), cudaMemcpyDeviceToHost);

    printf("a: %d, b: %d => c: %d\n", a, b, c);
    return 0;
}
