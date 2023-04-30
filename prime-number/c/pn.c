#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#ifdef VAR_TYPE_int
  #define VAR_TYPE int
  #define VAR_NAME "int"
  #define VAR_FMT "%d"
#endif

#ifdef VAR_TYPE_long
  #define VAR_TYPE long
  #define VAR_NAME "long"
  #define VAR_FMT "%ld"
#endif

const size_t NUM_BUF = 10 * 1000 * 1000;
const size_t BUF_SIZE = sizeof(VAR_TYPE) * NUM_BUF;

struct args {
    size_t upper_bound;
    int show_result;
    int realloc;
};

static void parse_option(int argc, char **argv, struct args *args)
{
    args->upper_bound = 100;
    args->show_result = 0;
    args->realloc = 0;

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
        else if (strcmp(word, "--realloc") == 0) {
            args->realloc = 1;
        }
    }
}

static int is_prime_number(
  const VAR_TYPE n, const VAR_TYPE *prime_numbers, const size_t num_prime_numbers)
{
    for (size_t i = 0; i < num_prime_numbers; i++) {
        const VAR_TYPE pn = prime_numbers[i];
        if (pn * pn > n)
            return 1;
        if (n % pn == 0)
            return 0;
    }
    return 1;
}

static void show_prime_numbers(
  const VAR_TYPE *prime_numbers, const size_t num_prime_numbers)
{
    for (size_t i = 0; i < num_prime_numbers; i++)
        printf(VAR_FMT " ", prime_numbers[i]);
    printf("\n");
}

static void show_result(
  const struct args *args,
  const VAR_TYPE *prime_numbers, const size_t num_prime_numbers)
{
    printf("Count: %zd\n", num_prime_numbers);
    if (args->show_result)
        show_prime_numbers(prime_numbers, num_prime_numbers);
}

static void run_fixed_length_array(struct args *args)
{
    VAR_TYPE *prime_numbers = malloc(BUF_SIZE);
    assert(prime_numbers);

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

    show_result(args, prime_numbers, num_prime_numbers);
}

static void run_realloc(struct args *args)
{
    VAR_TYPE *prime_numbers = realloc(NULL, sizeof(VAR_TYPE));
    assert(prime_numbers);

    size_t num_prime_numbers = 1;
    prime_numbers[0] = 2;
    for (size_t n = 3; n < args->upper_bound; n += 2) {
        if (is_prime_number(n, prime_numbers, num_prime_numbers)) {
            const size_t size = sizeof(VAR_TYPE) * (num_prime_numbers + 1);
            prime_numbers = realloc(prime_numbers, size);
            assert(prime_numbers);
            prime_numbers[num_prime_numbers] = n;
            num_prime_numbers++;
        }
    }

    show_result(args, prime_numbers, num_prime_numbers);
}

int main(int argc, char *argv[])
{
    struct args args;
    parse_option(argc, argv, &args);
    printf("Var type: " VAR_NAME ", size: %zd\n", sizeof(VAR_TYPE));
    printf("Upper bound: %zd\n", args.upper_bound);

    if (args.realloc == 0) {
        printf("Type: fixed-length array\n");
        run_fixed_length_array(&args);
    } else {
        printf("Type: realloc\n");
        run_realloc(&args);
    }

    return EXIT_SUCCESS;
}
