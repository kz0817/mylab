#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

const size_t NUM_BUF = 10 * 1000 * 1000;
const size_t BUF_SIZE = sizeof(int) * NUM_BUF;

struct args {
    size_t upper_bound;
    int show_result;
};

static void parse_option(int argc, char **argv, struct args *args)
{
    args->upper_bound = 100;
    args->show_result = 0;
    for (int i = 1; i < argc; i++) {
        const char *word = argv[i];
        if (strcmp(word, "-u") == 0) {
            i++;
            assert(i < argc);
            args->upper_bound = atoi(argv[i]);
        }
        else if (strcmp(word, "-s") == 0) {
            args->show_result = 1;
        }
    }
}

static int is_prime_number(const int n, const int *prime_numbers, const size_t num_prime_numbers)
{
    for (size_t i = 0; i < num_prime_numbers; i++) {
        const int pn = prime_numbers[i];
        if (pn * pn > n)
            return 1;
        if (n % pn == 0)
            return 0;
    }
    return 1;
}

static void show_result(const int *prime_numbers, const size_t num_prime_numbers)
{
    for (size_t i = 0; i < num_prime_numbers; i++)
        printf("%d ", prime_numbers[i]);
    printf("\n");
}

static void run(int *prime_numbers, const struct args *args)
{
    size_t num_prime_numbers = 1;
    prime_numbers[0] = 2;

    for (size_t n = 3; n < args->upper_bound; n += 2) {
        if (is_prime_number(n, prime_numbers, num_prime_numbers)) {
            prime_numbers[num_prime_numbers] = n;
            num_prime_numbers++;
        }
    }

    if (num_prime_numbers > NUM_BUF) {
        printf("*** WARNING *** Buffer overflow\n");
    }
    printf("Count: %zd\n", num_prime_numbers);
    if (args->show_result)
        show_result(prime_numbers, num_prime_numbers);
}

int main(int argc, char *argv[])
{
    struct args args;
    parse_option(argc, argv, &args);
    printf("Upper bound: %zd\n", args.upper_bound);

    int *prime_numbers = malloc(BUF_SIZE);

    if (prime_numbers == NULL) {
        fprintf(stderr, "Failed to allocate memory: %zd\n", BUF_SIZE);
        return EXIT_FAILURE;
    }

    run(prime_numbers, &args);

    return EXIT_SUCCESS;
}
