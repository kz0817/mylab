#!/usr/bin/env python
import argparse
import math

class WorkItem:
    def __init__(self, gid, data, sum_arr):
        self.gid = gid
        self.data = data
        self.sum_arr = sum_arr

    def step(self, i):
        if self.gid & (1 << i) == 0:
            return
        idx = (self.gid | ((1 << (i+1)) -  1)) - (1 << i)
        self.sum_arr[self.gid] += self.sum_arr[idx]

def prepare_data(concurrency):
    return [x + 1 for x in range(concurrency)]


def parallel_sum(concurrency, data):
    sum_arr = map(lambda n: n, data)
    work_items = [WorkItem(i, data, sum_arr) for i in range(concurrency)]
    n_loop = int(math.ceil(math.log(concurrency) / math.log(2)))
    for i in range(n_loop):
        for work_item in work_items:
            work_item.step(i)

    return sum_arr[concurrency-1]


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('-c', '--concurrency', type=int, default=10)
    args = parser.parse_args()
    data = prepare_data(args.concurrency)
    sum_simple = reduce(lambda a, b: a + b, data)
    sum_quick = parallel_sum(args.concurrency, data)
    print "Simple: %d, Quick: %d" % (sum_simple, sum_quick)
    assert sum_simple == sum_quick


if __name__ == "__main__":
    main()
