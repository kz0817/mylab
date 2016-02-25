#include <cstdio>
#include <vector>
#include <string>
using namespace std;

struct X {
	int            a;
	vector<string> v;
};

static const X a[] = {
{
	1,
	{"HOGE"},
}, {
	5,
	{"DOG", "CAT"},
},
};

int main(void)
{
	vector<int> a = {1};
	static const vector<string> s = {"HOGE"};
	return 0;
}
