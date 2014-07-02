#include <stdio.h>
#include <stdlib.h>

int add(int a, int b)
{
	return a + b;
}

int add_sync(int a, int b)
{
	return __sync_add_and_fetch(&a, b);
}

int main(void)
{
	printf("builtin_add: %d\n", add_sync(5, 3));
	printf("normal     : %d\n", add(5,3));
	return EXIT_SUCCESS;
}
