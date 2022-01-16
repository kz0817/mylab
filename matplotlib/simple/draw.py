#!/usr/bin/env python3
['line', 'points'])
import argparse
import numpy as np
import matplotlib.pyplot as plt


def draw_figure(args):

    def draw_line():
        x_vec = np.linspace(args.lower, args.upper, args.num_line_points)
        y_vec = np.sin(x_vec)
        plt.plot(x_vec, y_vec, label='line')

    def draw_points():
        x_vec = np.linspace(args.lower, args.upper, args.num_points)
        y_vec = np.sin(x_vec) + \
                args.noise_amp * (np.random.rand(args.num_points) - 0.5)
        plt.plot(x_vec, y_vec, '.', label='points')


    plt.xlabel("x")
    plt.xlim(args.lower, args.upper)
    plt.xticks(np.arange(args.lower, args.upper))

    plt.ylabel("y")
    plt.ylim(-1.2, 1.2)
    plt.yticks(np.arange(-1.2, 1.2, 0.2))

    plt.grid()

    draw_line()
    draw_points()
    plt.legend()

    plt.savefig(args.figure_name)

def run(args):
    draw_figure(args)

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('lower', type=float)
    parser.add_argument('upper', type=float)
    parser.add_argument('-l', '--num-line-points', type=int, default=100)
    parser.add_argument('-n', '--num-points', type=int, default=10)
    parser.add_argument('-k', '--noise-amp', type=float, default=1.0)
    parser.add_argument('-f', '--figure-name', default='figure.svg')
    args = parser.parse_args()

    run(args)


if __name__ == '__main__':
    main()
