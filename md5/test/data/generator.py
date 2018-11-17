#!/usr/bin/env python3
import argparse
import struct

def run(args):
    for i in range(args.size):
        args.out_file.write(struct.pack('B', i % 0x100))


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('-s', '--size', type=int, required=True)
    parser.add_argument('-o', '--out-file', type=argparse.FileType('wb'),
                        required=True)
    args = parser.parse_args()
    run(args)

if __name__ == '__main__':
    main()
