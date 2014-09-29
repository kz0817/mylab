#include <cstdio>
#include <cstdlib>

template <typename T>
class Ptr {
public:
	Ptr(T *ptr)
	: m_ptr(ptr)
	{
	}

	T *operator->() {
		return m_ptr;
	}

private:
	T *m_ptr;
};

struct Foo {
	int x;
};

int main(void)
{
	Foo foo;
	foo.x = 5;
	Ptr<Foo> fooPtr(&foo);
	fooPtr->x = 10;

	Ptr<const Foo> constFooPtr(&foo);
	printf("constFooPtr->x: %d\n", constFooPtr->x); // read OK.
	// constFooPtr->x = 20; // Write NG. Build error.

	const Ptr<Foo> constPtrFoo(&foo);
	printf("constPtrFoo->x: %d\n", constFooPtr->x); // read OK.
	constPtrFoo->x = 20; // Write OK.

	return EXIT_SUCCESS;
}
