#include <cstdio>
#include <cstdlib>
#include <string>
#include <boost/format.hpp>
using namespace std;
using namespace boost;

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

int func(int a)
{
	X s(str(format("%s:%d") % __func__ % a));
	if (a % 2)
		throw a;
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
	} catch (...) {
		printf("Catch an exception\n");
	}
	return EXIT_SUCCESS;
}
