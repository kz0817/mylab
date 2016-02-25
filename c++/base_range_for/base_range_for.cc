#include <cstdio>
#include <vector>

using namespace std;

int main(void)
{
	vector<int> v;
	v.push_back(3);
	v.push_back(4);

	printf("%p, %p\n", &v[0], &v[1]);

	printf("*** reference ***\n");
	for (const auto &data: v)
		printf(">> %p\n", &data);

	printf("*** w/o reference ***\n");
	for (const auto data: v)
		printf(">> %p\n", &data);

	return 0;
}
