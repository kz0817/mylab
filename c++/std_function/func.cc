#include <cstdio>
#include <functional>
using namespace std;

int main(void)
{
	function<int(int)> x;
	printf("%d\n", x(9));
	return 0;
}
