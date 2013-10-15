#include <cstdio>
#include <cstdlib>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>

using namespace std;

int main(int argc, char *argv[])
{
	if (argc < 2) {
		printf("You should add a file name in the command line.\n");
		return EXIT_FAILURE;
	}
	const char *filename = argv[1];
	int fd = open(filename, O_RDONLY);
	if (fd == -1) {
		printf("Failed to open: %s: errno: %d\n", filename, errno);
		return EXIT_FAILURE;
	}

	const size_t buf_size = 0x2000;
	char buf[buf_size];
	while (1) {
		ssize_t size = read(fd, buf, buf_size);
		if (size == -1) {
			printf("Failed to read: %d\n", errno);
			return EXIT_FAILURE;
		}
		if (size == 0)
			continue;
		string str(buf, size);
		printf("%s", str.c_str());
	}

	return EXIT_FAILURE;
}
