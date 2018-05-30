#!/usr/bin/env python

import argparse
from Crypto.Cipher import AES

def pad(msg, align):
    n_pads = align - len(msg) % align
    print "num padding: %s" % n_pads
    new_msg = msg + n_pads * chr(n_pads)
    return new_msg


def encrypt(args):
    crypto = AES.new(args.key, AES.MODE_CBC, args.iv)
    data = crypto.encrypt(pad(args.message, 16))
    return data.encode("hex")


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("message", type=str)
    parser.add_argument("-k", "--key", type=str, required=True)
    parser.add_argument("-v", "--iv", type=str, required=True)
    args = parser.parse_args()
    print "msg: %s" % args.message
    print "key: %s" % args.key
    print "iv : %s" % args.iv
    print encrypt(args)


if __name__ == "__main__":
    main()

