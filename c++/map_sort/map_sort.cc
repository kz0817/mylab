#include <cstdio>
#include <cstdlib>
#include <string>
#include <map>

using namespace std;

int main(void)
{
	map<int, string> mymap;
	mymap[3] = "hoge";
	mymap[1] = "foo";
	mymap[20] = "goo";
	mymap[10] = "Tea";
	mymap[-5] = "foo";

	map<int, string>::iterator it = mymap.begin();
	for (; it != mymap.end(); ++it) {
		printf("%d\t%s\n", it->first, it->second.c_str());
	}
	return EXIT_SUCCESS;
}
