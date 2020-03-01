#include <cstdio>
#include <string>
using namespace std;

class X {
	int    val;
	string name;

public:
	X(const X &x)
	{
		name = "No name from ";
		name += x.name;
		val = x.val;
		printf("Copy constructor [%s] %d\n", name.c_str(), val);
	}

	X(const string &n, int v)
	: name(n),
	  val(v)
	{
		printf("Default constructor w/ Name [%s] %d\n", name.c_str(), val);
	}

	X(const string &n, const X &x)
	: name(n)
	{
		val = x.val;
		printf("Copy constructor w/ Name [%s] %d\n", name.c_str(), val);
	}

	X(const string &n, X &&x)
	: name(n)
	{
		val = x.val;
		printf("Move constructor [%s] %d\n", name.c_str(), val);
	}

	X &operator=(const X &rhs)
	{
		printf("Operator= : copy [%s:%d] -> [%s:%d]\n",
		       rhs.name.c_str(), rhs.val, name.c_str(), val);
		val = rhs.val;
		return *this;
	}

	X &operator=(X &&rhs)
	{
		printf("Operator= : move [%s:%d] -> [%s:%d]\n",
		       rhs.name.c_str(), rhs.val, name.c_str(), val);
		val = rhs.val;
		return *this;
	}

    virtual ~X() {
        printf("Desctructor: %s\n", name.c_str());
    }

	void print(void)
	{
		printf("val: %d\n", val);
	}
};

X func1(void)
{
	X v0(__func__, 5);
	return v0;
}

const X func_const(void)
{
	return func1();
}

int main(void)
{
	printf("[x1] ==========\n");
	X x1("x1", 1);
	printf("\n");

	printf("[x2] ==========\n");
	X x2("x2", x1);
	printf("\n");

	printf("[x2a] ==========\n");
	X x2a("x2a", move(x1));
	printf("\n");

	printf("[x3] ==========\n");
	X x3("x3", 3);
	x3 = func1();
	printf("\n");

	printf("[x3a] ==========\n");
	X x3a("x3a", func1());
	printf("\n");

	printf("[x4] ==========\n");
	X x4("x4", 4);
	x4 = func_const();
	printf("\n");

	printf("[x5l] ==========\n");
	const X& x5l = func1();
	printf("\n");

	printf("[x5lc] ==========\n");
	const X& x5lc = func_const();
	printf("\n");

	printf("[x5r] ==========\n");
	X&& x5r = func1();
	printf("\n");

	printf("[x5rc] ==========\n");
	const X&& x5rc = func_const();
	printf("\n");
}
