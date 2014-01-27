#include <cstdio>
#include <cstdlib>
#include <vector>
using namespace std;

class A {
public:
	A(void)
	{
		printf("A: constructor\n");
	}
	
	A(const A &)
	{
		printf("A: copy constructor\n");
	}
};

typedef vector<A> AVec;
typedef AVec::iterator AVecIter;

int main(void)
{
	AVec avec;
	avec.push_back(A());
	avec.push_back(A());
	avec.push_back(A());
	printf("--------------\n");
	AVecIter it = avec.begin();
	for (; it != avec.end(); ++it)
		printf("%p\n", &*it);
	printf("--------------\n");
	it = avec.begin();
	for (; it != avec.end(); it++)
		printf("%p\n", &*it);
	
	return EXIT_SUCCESS;
}
