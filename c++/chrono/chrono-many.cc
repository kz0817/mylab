#include <cstdlib>
#include <chrono>

using namespace std;

int main(int argc, char *argv[])
{
	const size_t nloop = atol(argv[1]);
	chrono::system_clock::time_point t;
	for (size_t i = 0; i < nloop; i++)
		t = std::chrono::system_clock::now();
}
