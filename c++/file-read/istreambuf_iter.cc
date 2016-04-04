#include <cstdio>
#include <cstdlib>
#include <fstream>
using namespace std;

void showFileContent(const char *filename)
{
	ifstream ifs(filename);
	istreambuf_iterator<char> it(ifs);
	istreambuf_iterator<char> last;
	for (; it != last; ++it)
		printf("%c", *it);
}

int main(int argc, char *argv[])
{
	if (argc < 2) {
		fprintf(stderr, "Need a file path.\n");
		return EXIT_FAILURE;
	}
	const char *filename = argv[1];
	showFileContent(filename);
	return EXIT_SUCCESS;
}
