#include <stdio.h>
#include <stdlib.h>

#define BUF_SIZE 100

int main(void)
{
	char name[BUF_SIZE];
	printf("What are your name ? : ");
	fgets(name, BUF_SIZE, stdin);
	printf("Hello, %s\n", name);
	return EXIT_SUCCESS;
}
