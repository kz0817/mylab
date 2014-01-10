#include <cstdio>

class X {
	int a;
public:
	X(int _a)
	: a(_a)
	{
		printf("default constructor: %d\n", a);
	}

	X(const X &x)
	{
		printf("copy constructor\n");
	}

	X(X &&x)
	{
		printf("move constructor\n");
	}

	X &operator=(X &&rhs)
	{
		printf("ope = &&\n");
		a = rhs.a;
		return *this;
	}

	void print(void)
	{
		printf("a: %d\n", a);
	}
};

X func(void)
{
	X t0(5);
	return t0;
}

const X func_const(void)
{
	return func();
}

int main(void)
{
	X x1(1);
	X x2(x1);
	X x3(3);
	x3 = func();
	x3.print();
	X x4(3);
	// x4 = func_const(); // build error

}
