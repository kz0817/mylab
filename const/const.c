#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	const char *a = "A";
	char * const b = "B";
	char const * c = "C";
	a = "A2";
	// b = "B2"; // Build error
	c = "C2";
	// a[0] = 'X'; // Build error
	b[0] = 'X';
	// c[0] = 'X'; // Build error
	return EXIT_FAILURE;
}
