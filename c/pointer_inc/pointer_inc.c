#include <stdio.h>

void inc(int *a)
{
	(*a)++;
}

int main(void)
{
	int a = 5;
	printf("a: %d\n", a);
	inc(&a);
	printf("a: %d\n", a);
}
