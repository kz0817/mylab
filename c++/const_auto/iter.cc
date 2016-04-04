#include <cstdio>
#include <list>
using namespace std;

int main(void)
{
	list<int> hoge = {2, 5, 1};

	auto it = hoge.begin();
	printf("auto: name: %s\n", typeid(it).name());

	list<int>::iterator it2 = hoge.begin();
	printf("iter: name: %s\n", typeid(it2).name());

	const auto it3 = hoge.begin();
	printf("const auto: name: %s\n", typeid(it3).name());

	list<int>::const_iterator it4 = hoge.begin();
	printf("const_iter: name: %s\n", typeid(it4).name());

	auto it5 = ((const list<int> &)hoge).begin();
	printf("cast const: name: %s\n", typeid(it5).name());

	auto it6 = hoge.cbegin();
	printf("auto cbegin: name: %s\n", typeid(it6).name());

	return 0;
}
