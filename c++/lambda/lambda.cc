#include <cstdio>

class Foo {
public:
	int run(int a) {
		auto doIt = [&] {
			printf("a: %d, goo: %d\n", a, this->goo());
			return 0;
		};
		return doIt();
	}

private:
	int goo(void)
	{
		return 46;
	}

};

int main(void)
{
	Foo f;
	return f.run(5);
}
