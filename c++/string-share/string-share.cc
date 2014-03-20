#include <cstdio>
#include <cstdlib>
#include <string>
using namespace std;

int main(void)
{
	string a = "ABC";
	printf("&a : %p, %p\n", &a, a.c_str());

	string b(a);
	printf("&b : %p, %p\n", &b, b.c_str());

	string c = "foo";
	printf("&c : %p, %p\n", &c, c.c_str());

	c = a;
	printf("&c': %p, %p\n", &c, c.c_str());

	return EXIT_SUCCESS;
}
