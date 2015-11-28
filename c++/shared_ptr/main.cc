#include <cstdio>
#include <memory>
using namespace std;

class X {
	int a;
public:
	X(int _a)
	: a(_a)
	{
	}

	virtual ~X()
	{
		printf("Destructor: %d\n", a);
	}

	void print(void)
	{
		printf("a: %d\n", a);
	}
};

int main(void)
{
	shared_ptr<X> xptr;
	xptr.reset(new X(5));
	xptr.reset(new X(22));
	xptr->print();
	shared_ptr<X> yptr = xptr;
	return 0;
}
