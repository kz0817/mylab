#include <string>
#include <cstdio>
using namespace std;

class Mame {
	string name;

public:
	Mame(const char *_name) {
		name = _name;
		printf("[%s]\tConstructor: const char *\n", name.c_str());
	}

	Mame(const string &_name) {
		name = _name;
		printf("[%s]\tConstructor: const string &_name\n", name.c_str());
	}
};

int main(void)
{
	Mame m1("m1");
	string m2str = "m2";
	Mame m2(m2str);
	Mame m3(string("m3"));

	return 0;
}
