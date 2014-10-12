#include <cstdio>
#include <cstdlib>
#include <vector>
using namespace std;

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
	void foo(void)
	{
	}

	void fooConst(void) const
	{
	}
};

int main(void)
{
	Foo foo;
	foo.x = 5;
	Ptr<Foo> fooPtr(&foo);
	fooPtr.get()->x = 10;
	fooPtr.get()->foo();
	fooPtr.get()->fooConst();
	fooPtr.get_const()->foo();
	fooPtr.get_const()->fooConst();

	Ptr<const Foo> constFooPtr(&foo);
	printf("constFooPtr->x: %d\n", constFooPtr.get()->x); // read OK.
	// constFooPtr->x = 20; // Write NG. Build error.
	// constFooPtr.get_const()->x = 20; // NG. Build error.

	//constFooPtr.get()->foo(); // NG.
	constFooPtr.get()->fooConst();
	// constFooPtr.get_const()->foo(); NG
	constFooPtr.get_const()->fooConst();

	const Ptr<Foo> constPtrFoo(&foo);
	printf("constPtrFoo->x: %d\n", constFooPtr.get()->x); // read OK.
	constPtrFoo.get_const()->x = 20; // OK.

	//constPtrFoo.get()->foo(); NG.
	//constPtrFoo.get()->fooConst(); NG.
	constPtrFoo.get_const()->foo();
	constPtrFoo.get_const()->fooConst();

	vector<int> intVec;
	intVec.push_back(5);

	const vector<int> constIntVec;
	constIntVec.push_back(5);

	return EXIT_SUCCESS;
}
