#include <cstdio>
#include <string>
using namespace std;

int main(int argc, char *argv[])
{
	string s = "This is a book.";
	string t(s.begin()+1, s.begin()+6);
	printf("t: %s\n", t.c_str());
}
