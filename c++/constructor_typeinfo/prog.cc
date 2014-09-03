#include <cstdio>
#include <cstdlib>
#include <typeinfo>

using namespace std;

class ClassA {
public:
	ClassA(const type_info &type);
};

class ClassB : public ClassA {
public:
	ClassB(void)
	: ClassA(typeid(*this))
	{
		printf("ClassB: %s\n", typeid(*this).name());
	}
};

ClassA::ClassA(const type_info &type)
{
	printf("ClassA: %s (%s), isClassB: %d\n", typeid(*this).name(), type.name(), type == typeid(ClassB));
}

int main(void)
{
	ClassB b;
	return EXIT_SUCCESS;
}
