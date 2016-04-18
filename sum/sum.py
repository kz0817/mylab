#!/usr/bin/env python
import argparse

def prepare_data(concurrency):
    return [x + 1 for x in range(concurrency)]


def quick_sum(concurrency, data):
    return 0


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('-c', '--concurrency', type=int, default=10)
    args = parser.parse_args()
    data = prepare_data(args.concurrency)
    print "Simple: %d" % reduce(lambda a, b: a + b, data)
    print "Simple: %d" % quick_sum(args.concurrency, data)


if __name__ == "__main__":
    main()
