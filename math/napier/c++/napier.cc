#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <limits.h>

int main() {
    for (long long n = 1; n > 0 && n < LONG_MAX; n *= 10) {
        const auto e = powl(1 + 1.0/n, n);
        printf("%20lld %1.14Lf\n", n, e);
    }
    return EXIT_SUCCESS;
}
