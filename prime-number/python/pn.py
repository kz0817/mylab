#!/usr/bin/env python

import argparse

def calc(args):
    prime_numbers = []

    def is_prime_number(n):
        for pn in prime_numbers:
            if pn * pn > n:
                break
            if n % pn == 0:
                return False
        return True


    for n in range(2, args.upper_limit + 1):
        if is_prime_number(n):
            prime_numbers.append(n)

    print "Count: %s" % len(prime_numbers)
    if args.show:
        for pn in prime_numbers:
            print pn


def start():
    parser = argparse.ArgumentParser()
    parser.add_argument("-u", "--upper-limit", type=int)
    parser.add_argument("-s", "--show", action="store_true")
    args = parser.parse_args()

    print "Upper limit: %s" % args.upper_limit
    calc(args)

if __name__ == "__main__":
    start()
