#!/usr/bin/env python3
import argparse
import cmath
import numpy as np

def create_data(num_points):
    return np.arange(num_points)


def show_points(x: complex, header=None):
    if header is not None:
        print(header)
    for i in range(x.size):
        a = x[i]
        print('%+.3f, %+.3f' % (a.real, a.imag))


def dft(data, inverse=False):
    n = data.size
    x = np.zeros(n, dtype=np.complex)
    a = -2j*cmath.pi/n
    if inverse:
        a *= -1
    for k in range(n):
        for j in range(n):
            x[k] += data[j] * cmath.exp(a*j*k)
        if inverse:
            x[k] /= n
    return x


def run_fft(data, w):
    n = data.size
    assert n%2 == 0
    x = np.zeros(n, dtype=np.complex) # An array for the output
    if n == 2:
        x[0] = data[0] + data[1];
        x[1] = data[0] + w * data[1];
        return x

    w_sq = w*w
    n_half = n // 2

    # even and odd respectively has n/2 elements
    even = run_fft(data[::2], w_sq)
    odd = run_fft(data[1::2], w_sq)

    for k in range(n_half):
        x[k] = even[k] + (w**k) * odd[k]
        l = k + n_half
        x[l] = even[k] + (w**l) * odd[k]

        # We can also write as below
        # c = w**k
        # x[k] = even[k] + c * odd[k]
        # x[l] = even[k] - c * odd[k]

    return x


def fft(data, inverse=False):
    n = data.size
    sign = -1 if inverse else 1
    w = cmath.exp(2j*sign*cmath.pi/n)

    x = run_fft(data, w)
    if inverse:
        x /= n

    return x


def run(args):
    operation = 'FFT' if args.do_fft else 'DFT'

    data = create_data(args.num_points)
    if not args.silent:
        print('data: %s' % data)
    if args.do_fft:
        x = fft(data)
    else:
        x = dft(data)
    if not args.silent:
        show_points(x, header='%s:' % operation)

    if args.do_fft:
        y = fft(x, inverse=True)
    else:
        y = dft(x, inverse=True)
    if not args.silent:
        show_points(y, header='I%s:' % operation)


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('-n', '--num-points', type=int, default=4)
    parser.add_argument('-f', '--do-fft', action='store_true')
    parser.add_argument('-s', '--silent', action='store_true')
    args = parser.parse_args()

    run(args)


if __name__ == '__main__':
    main()
