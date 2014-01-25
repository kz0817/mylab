#include <cstdio>
#include <cstdlib>
#include <string>
using namespace std;

int main(void)
{
	string a;
	printf("Point 1: sizeof(a string instance): %zd\n", sizeof(a));
	asm volatile("":::"memory");
	a = "Test message.";
	printf("Point 2: %s\n", a.c_str());
	return EXIT_SUCCESS;
}
