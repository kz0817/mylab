#!/usr/bin/env python3
import argparse
import math

class Element(object):
    def __init__(self, n, v):
        self.n = n # order
        if isinstance(v, Element):
            v = v.v
        self.v = v % n

    def __add__(lhs, rhs):
        if isinstance(rhs, Element):
            assert lhs.n == rhs.n
            _rhs = rhs.v
        else:
            _rhs = rhs
        return Element(lhs.n, lhs.v + _rhs)

    def __sub__(lhs, rhs):
        assert lhs.n == rhs.n
        return Element(lhs.n, lhs.v - rhs.v)

    def __mul__(lhs, rhs):
        assert lhs.n == rhs.n
        return Element(lhs.n, lhs.v * rhs.v)

    def __rmul__(lhs, rhs):
        assert isinstance(lhs, Element)
        assert isinstance(rhs, int)
        return Element(lhs.n, lhs.v * rhs)

    def __truediv__(lhs, rhs):
        assert lhs.n == rhs.n
        n = lhs.n
        if rhs.v == 0:
            raise ZeroDivisionError

        for v in range(n):
            if ((v * rhs.v) % n) == (lhs.v % n):
                return Element(n, v)
        msg = 'No element: %d / %d (mod %d)' % (lhs.v, rhs.v, n)
        raise ArithmeticError(msg)

    def __neg__(self):
        self.v *= -1
        self.v %= self. n
        return self

    def __pow__(lhs, rhs):
        assert isinstance(lhs, Element)
        assert isinstance(rhs, int)
        v = Element(lhs.n, 1)
        for i in range(rhs):
            v = v * lhs
        return v

    def __str__(self):
        return '%d(n=%d)' % (self.v, self.n)

    def sqrt(self):
        ans = []
        for v in range(self.n):
            if ((v * v) % self.n) == self.v:
                ans.append(v)
        return ans


class Point(object):
    def __init__(self, x: Element, y: Element):
       self.x = x
       self.y = y

    def __str__(self):
        return '(%d, %d)' % (self.x.v, self.y.v)

    def __eq__(lhs, rhs):
        assert isinstance(rhs, Point)
        return (lhs.x == rhs.x) and (lhs.y == rhs.y)


class InfinityPoint(object):
    def __str__(self):
        return 'InfinityPoint'


class Curve(object):
    def __init__(self, n, a, b):
        self.n = n # order
        self.a = a
        self.b = b

    def y2(self, x):
        _y2 = int(math.pow(x, 3)) + self.a * x + self.b
        y2 = Element(self.n, _y2)
        return y2

    def double(self, p: Point):
        assert isinstance(p, Point)
        def calc_phi() -> Element:
            s = Element(self.n, 3*(p.x**2) + self.a)
            t = Element(self.n, 2*p.y)
            return s / t

        def calc_psi() -> Element:
            s = Element(self.n, -3*(p.x**3) -self.a*p.x + 2*(p.y**2))
            t = Element(self.n, 2*p.y)
            return s / t

        phi = calc_phi()
        psi = calc_psi()
        x = phi**2 - 2*p.x
        y = -phi*x - psi
        return Point(x, y)

    def add(self, p1: Point, p2: Point):
        if isinstance(p1, InfinityPoint) and isinstance(p2, InfinityPoint):
            return InfinityPoint()
        if isinstance(p1, InfinityPoint):
            return Point(p2.x, p2.y)
        if isinstance(p2, InfinityPoint):
            return Point(p1.x, p1.y)

        assert isinstance(p1, Point)
        assert isinstance(p2, Point)

        if p1 == p2:
            return self.double(p1)

        denom = Element(self.n, p2.x - p1.x)
        if denom.v == 0:
            return InfinityPoint()
        phi = Element(self.n, p2.y - p1.y) / denom
        psi = Element(self.n, p1.y*p2.x - p2.y*p1.x) / denom
        x = phi**2 - p1.x - p2.x
        y = -phi*x - psi
        return Point(x, y)

    def mutiply(self, k: int, base_point: Point) -> Point:
        if k == 1:
            return base_point

        p = self.double(base_point)
        if k == 2:
            return p

        for i in range(k-2):
            p = self.add(p, base_point)
        return p


def list_points(args):
    curve = Curve(args.order, args.a, args.b)
    for x in range(args.order):
        y2 = curve.y2(x)
        y = y2.sqrt()
        print(x, y2, y)


def create_point(n, x_y):
    _x, _y = x_y
    x = Element(n, _x)
    y = Element(n, _y)
    return Point(x, y)


def operate(args):
    g = create_point(args.order, args.base_point)
    curve = Curve(args.order, args.a, args.b)

    for k in range(args.multiplication_times):
        p = curve.mutiply(k + 1, g)
        print(p)


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('-o', '--order', type=int, default=5)
    parser.add_argument('-a', type=int, default=0)
    parser.add_argument('-b', type=int, default=7)

    subparsers = parser.add_subparsers(dest='subparser')
    parser_list = subparsers.add_parser('list')
    parser_list.set_defaults(function=list_points)

    parser_op = subparsers.add_parser('operate')
    parser_op.add_argument('base_point', type=int, nargs=2)
    parser_op.add_argument('-d', '--multiplication-times', type=int, default=3)
    parser_op.set_defaults(function=operate)

    args = parser.parse_args()
    args.function(args)


if __name__ == '__main__':
    main()
