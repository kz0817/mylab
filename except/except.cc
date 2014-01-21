#include <cstdio>
#include <cstdlib>
#include <string>
#include <boost/format.hpp>
using namespace std;
using namespace boost;

class A {
public:
	A(void)
	{
		printf("A: const\n");
	}

	virtual ~A(void)
	{
		printf("A: dest\n");
	}
};

class X {
public:
	X(const string &name)
	: m_name(name)
	{
		printf("Constructor: %s\n", m_name.c_str());
	}

	virtual ~X()
	{
		printf("Destructor: %s\n", m_name.c_str());
	}

private:
	const string m_name;
};

string createName(const string &funcName, const int &val)
{
	return str(format("%s:%d") % funcName % val);
}

int func(int a)
{
	X s(createName(__func__, a));
	if (a % 2)
		throw A();
	return a * 3;
}

int main(int argc, char *argv[])
{
	if (argc < 2) {
		printf("Needs an integer as an argument.\n");
		return EXIT_FAILURE;
	}

	int val = atoi(argv[1]);

	try {
		printf("%d\n", func(val));
	} catch (const A &e) {
		printf("Catch A exception\n");
	} catch (...) {
		printf("Catch an exception\n");
	}
	pintf("Go exit\n");
	return EXIT_SUCCESS;
}
