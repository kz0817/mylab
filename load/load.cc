#include <cstdio>
#include <cstdlib>
#include <string>
#include <pthread.h>
#include <errno.h>

using namespace std;

static void print_usage(void)
{
	printf("Usage:\n");
	printf("\n");
	printf("$ load [--num-threads N]\n");
	printf("\n");
}

static void *loop(void *arg)
{
	while (true)
		;
}

int main(int argc, char *argv[])
{
	int num_threads = 1;
	for (int i = 1; i < argc; i++) {
		string arg = string(argv[i]);
		bool lastArg = (i == argc - 1);
		if (arg == "--help" || arg == "-h") {
			print_usage();
			return EXIT_SUCCESS;
		}
		else if (arg == "--num-threads") {
			if (lastArg) {
				print_usage();
				return EXIT_FAILURE;
			}
			i++;
			num_threads = atoi(argv[i]);
		}
	}
	printf("Number of threads: %d\n", num_threads);
	for (int i = 1; i < num_threads; i++) {
		pthread_t thread;
		int result = pthread_create(&thread, NULL, loop, NULL);
		if (result != 0) {
			printf("Failed to create thread\n");
			return EXIT_FAILURE;
		}
	}
	loop(NULL);
	return EXIT_SUCCESS;
}
