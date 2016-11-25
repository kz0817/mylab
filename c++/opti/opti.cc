#include <iostream>
#include <cstdlib>
#include <string>
using namespace std;

int main(int argc, char *argv[])
{
	long sum = 0;
	long nloop = 10;
	for (int i = 0; i < argc; i++) {
		string arg = argv[i];
		nloop = atol(arg.c_str());
	}

	cout << "nloop: " << nloop << endl;
	for (long i = 0; i < nloop; i++)
		sum += i;
	cout << "sum: " << sum << endl;
	return EXIT_SUCCESS;
}

