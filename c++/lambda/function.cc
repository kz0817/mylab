#include <cstdio>
#include <functional>
using namespace std;

void registerCallback(function<int (void)> func)
{
	func();
}

class Foo {
public:
	int run(void) {
		int a = 5;
		auto func = [&] {
			printf("a: %d, obj: %p\n", a, this);
			return 1;
		};
		registerCallback(func);
		return 0;
	}
};

int main(void) {
	Foo f;
	return f.run();
}
