#include <stdio.h>
#include <cutil.h>

int main (void)
{
    cudaDeciceProp prop;
    cudaGetDeviceProperties(&prop, 0);
    printf("name: %s\n", prop.name);
    return 0;
}

