#include <cstdio>
#include <list>

using namespace std;

struct Foo {
	int a;

	Foo (int _a)
	: a(_a)
	{
	}
};

int main(void)
{
	list<Foo> aList;
	aList.push_back(Foo(1));
	aList.push_back(Foo(5));
	aList.push_back(Foo(3));

	const list<Foo> &aRef = aList;
	Foo &foo = const_cast<Foo &>(*aRef.begin());
	aRef.push_back(Foo(-5));

	printf("val: %d\n", foo.a);
}
