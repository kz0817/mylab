#!/usr/bin/env python3
import argparse

def run(args):
    num_trial = 0
    for n100 in range(1, args.total-1):
        n50_max = args.total - n100 - 1
        assert n50_max >= 1
        for n50 in range(1, n50_max+1):
            n10 = args.total - n100 - n50
            assert n10 >= 1
            assert n100 + n50 + n10 == args.total
            s = n100 * 100 + n50 * 50 + n10 * 10
            num_trial += 1
            if s == args.search_sum:
                print(f'n100: {n100}, n50: {n50}, n10: {n10}')
            elif s > args.search_sum:
                break

    print(f'num_trial: {num_trial}')


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--total', default=45)
    parser.add_argument('--search-sum', default=900)
    args = parser.parse_args()

    run(args)

if __name__ == '__main__':
    main()
