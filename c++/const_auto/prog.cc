#include <cstdio>

int main(void)
{
	int x = 5;
	const auto &a = x;
	printf("a: %d\n", a);
	// a = 3; // NG: build error
	return 0;
}
