#include <stdio.h>

int main(int argc, char *argv[])
{
	printf(" argv: %p\n", argv);
	printf("&argv: %p\n", &argv);

	int array[3];
	printf(" array: %p\n", array);
	printf("&array: %p\n", &array);

	char *arrv[5];
	printf(" arrv: %p\n", arrv);
	printf("&arrv: %p\n", &arrv);


	return 0;
}
