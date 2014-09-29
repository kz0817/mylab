#include <cstdio>
#include <cstdlib>

template <typename T>
class Ptr {
public:
	Ptr(T *ptr)
	: m_ptr(ptr)
	{
	}

	T *get(void) {
		return m_ptr;
	}

	T *get_const(void) const {
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
	fooPtr.get()->x = 10;

	Ptr<const Foo> constFooPtr(&foo);
	printf("constFooPtr->x: %d\n", constFooPtr.get()->x); // read OK.
	// constFooPtr->x = 20; // Write NG. Build error.
	// constFooPtr.get_const()->x = 20; // NG. Build error.

	const Ptr<Foo> constPtrFoo(&foo);
	printf("constPtrFoo->x: %d\n", constFooPtr.get()->x); // read OK.
	// constPtrFoo.get()->x = 20; // Write NG. Build error
	constPtrFoo.get_const()->x = 20; // OK.

	return EXIT_SUCCESS;
}
