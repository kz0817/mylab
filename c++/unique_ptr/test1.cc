#include <memory>
#include <iostream>
#include <vector>
#include <map>
#include <cstdlib>

using namespace std;

struct X {
	int a;

	X(int _a)
	: a(_a)
	{
		cout << "Constructor X: " << a << endl;
	}

	virtual ~X() {
		cout << "Destructor X: " << a << endl;
	}

	int get(void) {
		return a;
	}
};

auto func1ref(unique_ptr<X> &xref)
{
	return xref->a;
}

auto func1act(unique_ptr<X> xref)
{
	return xref->a;
}

auto generator(int a)
{
	auto x = make_unique<X>(a);
	return x;
}

auto func3ref(map<int, unique_ptr<X>> &x)
{
	return x.size();
}

auto func3act(map<int, unique_ptr<X>> x)
{
	return x.size();
}

template<typename T>
struct Z {
	T t;
};

auto func4_0(int a)
{
	Z<map<string, unique_ptr<X>>> z;
	z.t.insert(pair<string,unique_ptr<X>>("hoge", make_unique<X>(a)));
	return z;
}

auto func4(int a)
{
	return func4_0(a).t;
	/*
	Z<map<string, unique_ptr<X>>> z;
	z.t.insert(pair<string,unique_ptr<X>>("hoge", make_unique<X>(a)));
	return move(z.t);
	*/
}

int main(void)
{
	auto x = make_unique<X>(5);
	cout << "func1ref: " << func1ref(x) << endl;
	// [error] cout << "func1act: " << func2(x) << endl;
	cout << "func1act: " << func1act(move(x)) << endl;

	auto x2 = generator(3);
	cout << "x2: " << x2->get() << endl;

	map<int, unique_ptr<X>> m;
	cout << "func3ref: " << func3ref(m) << endl;
	// [error] cout << "func3act: " << func3ref(m) << endl;
	cout << "func3act: " << func3act(move(m)) << endl;

	auto z = func4(8);
	cout << "func4: " << z.size() << endl;


	return EXIT_SUCCESS;
}
