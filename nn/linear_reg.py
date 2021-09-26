#!/usr/bin/env python3
import argparse
import numpy as np
import matplotlib
matplotlib.use('Agg') 
import matplotlib.pyplot as plt
import math


class Perceptron(object):
    def __init__(self, n_input, learning_factor):
        self.n_input = n_input
        self.learning_factor = learning_factor
        self.w = np.zeros(n_input + 1)

    def learn(self, x_arr, t_arr):
        d_w = np.zeros(self.w.size)
        for i, x in enumerate(x_arr):
            v = 2 * (self.w[0] + self.w[1] * x - t_arr[i])
            #print('i: %d, x: %.3f, v: %s' % (i, x, v))
            d_w[0] += v
            d_w[1] += v * x

        self.w -= self.learning_factor * d_w

    def get_weight(self):
        return self.w

    @staticmethod
    def activate(x):
        return x if x >= 0 else 0

    @classmethod
    def calc(cls, w0, w1, x1):
        return w0 + w1 * x1


class Model(object):
    def __init__(self, param, threshold):
        self.a, self.b = param
        self.threshold = threshold

    def get_val(self, x) -> (float, float):
        val = self.a * x + self.b
        answer = 1.0 if val >= self.threshold else 0
        return val, answer


class LossFigure(object):
    def __init__(self):
        pass


def draw_model(ax, x_arr, y_arr):
    ax.plot(x_arr, y_arr)
    ax.set_xlabel("x")
    ax.set_ylabel("y")
    ax.set_title("model")


def draw_loss(ax, w_arr, loss_arr, log=False):
    z = np.log(loss_arr) if log else loss_arr
    ax.contourf(w_arr, w_arr, z)
    ax.set_xlabel('w0')
    ax.set_ylabel('w1')
    title = 'log(loss)' if log else 'loss'
    ax.set_title(title)

def calc_loss(x_arr, y_arr, w0, w1):
    loss = 0
    for k, x in enumerate(x_arr):
        v = Perceptron.calc(w0, w1, x)
        loss += math.pow(v - y_arr[k], 2)
    return loss


def learn(p: Perceptron, x_arr, t_arr, num_learning):
    w0_hist = np.zeros(num_learning)
    w1_hist = np.zeros(num_learning)
    loss_hist = np.zeros(num_learning)
    for i in range(num_learning):
        p.learn(x_arr, t_arr)
        w = p.get_weight()
        loss = calc_loss(x_arr, t_arr, *w)

        w0_hist[i], w1_hist = w
        loss_hist[i] = loss

    return w0_hist, w1_hist, loss_hist


def draw_loss_hist(ax, loss_hist):
    ax.plot(loss_hist)
    ax.set_xlabel('iteration')
    ax.set_ylabel('loss')
    ax.set_title('loss history')


def run(args):
    model = Model(args.model_param, args.threshold)
    p = Perceptron(1, args.learn_factor)
    fig, axes = plt.subplots(3, 2, tight_layout=True)

    x_arr = np.arange(args.lower, args.upper, step=args.step_size)
    y_arr = np.zeros(x_arr.size)
    t_arr = np.zeros(x_arr.size)
    for i, x in enumerate(x_arr):
        y_arr[i], t_arr[i] = model.get_val(x)
    draw_model(axes[0, 0], x_arr, y_arr)

    w_arr = np.arange(*args.weight_range, args.weight_step)
    loss_arr = np.zeros((w_arr.size, w_arr.size))
    for i, w0 in enumerate(w_arr):
        for j, w1 in enumerate(w_arr):
            loss_arr[j][i] = calc_loss(x_arr, y_arr, w0, w1)
    draw_loss(axes[1, 0], w_arr, loss_arr)
    draw_loss(axes[1, 1], w_arr, loss_arr, log=True)


    w0_hist, w1_hist, loss_hist = learn(p, x_arr, y_arr, args.num_learning)
    draw_loss_hist(axes[2, 0], loss_hist)

    plt.savefig(args.figure_name)


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('-l', '--lower', type=float, default=0)
    parser.add_argument('-u', '--upper', type=float, default=1)
    parser.add_argument('-p', '--model-param', nargs=2, type=float,
                        default=[0.5, -0.2])
    parser.add_argument('-n', '--num-samples', type=int, default=10)
    parser.add_argument('-e', '--learn-factor', type=float, default=0.1)
    parser.add_argument('-nl', '--num-learning', type=int, default=10)
    parser.add_argument('-t', '--threshold', type=float, default=0.5)
    parser.add_argument('-s', '--step-size', type=float, default=0.1)
    parser.add_argument('-w', '--weight-range', nargs=2, type=float,
                        default=[-1, 1])
    parser.add_argument('-ws', '--weight-step', type=float, default=0.1)
    parser.add_argument('-f', '--figure-name', default='figure.png')
    args = parser.parse_args()

    plt.rcParams['figure.figsize'] = (210/25.4, 297/25.4)
    run(args)

if __name__ == '__main__':
    main()
