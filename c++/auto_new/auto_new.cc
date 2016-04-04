#include<cstdio>

struct Foo {
	int x;
	void run(void) {
		printf("x: %d\n", x);
	}
};

int main(void)
{
	auto a = new Foo();
	a->x = 5;
	a->run();
	return 0;
}
