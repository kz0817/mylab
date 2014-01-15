#include <cstdio>

struct Xstruct {
	int a;
	int b;
};

static constexpr Xstruct xs[] = {
  {1, 2}, {10, 100}, {-5, 5}
};

static const Xstruct cs[] = {
  {1, 2}, {10, 100}, {-5, 5}
};

template<int N>
void func(void)
{
	printf("Your N: %d\n", N);
}

int main(void)
{
	constexpr int param = xs[0].a;
	func<param>();

	printf("===============\n");
	const int param2 = xs[1].a;
	func<param2>();

	printf("===============\n");
	const int param3 = cs[1].a;
	// func<param3>(); // Error

	return 0;
}
