#!/usr/bin/env python3
import unittest
import zp

class TestZp(unittest.TestCase):
    def test_add(self):
        data = (
            [5, 1, 1, 2],
            [5, 1, 4, 0],
            [5, -1, 0, 4],
            [5, -1, -4, 0],
            [5, -2, -4, 4],
        )
        for (n, _a, _b, expected) in data:
            with self.subTest(n=n, _a=_a, _b=_b, expected=expected):
                a = zp.Element(n, _a)
                b = zp.Element(n, _b)
                self.assertEqual((a + b).v, expected)
                self.assertEqual(a.v, _a % n)
                self.assertEqual(b.v, _b % n)

    def test_sub(self):
        data = (
            [5, 1, 1, 0],
            [5, 1, 4, 2],
            [5, -1, 0, 4],
            [5, -1, -4, 3],
            [5, -2, -4, 2],
        )
        for (n, _a, _b, expected) in data:
            with self.subTest(n=n, _a=_a, _b=_b, expected=expected):
                a = zp.Element(n, _a)
                b = zp.Element(n, _b)
                self.assertEqual((a - b).v, expected)
                self.assertEqual(a.v, _a % n)
                self.assertEqual(b.v, _b % n)

    def test_mul(self):
        data = (
            [5, 1, 1, 1],
            [5, 1, 4, 4],
            [5, 2, 4, 3],
            [5, -1, -4, 4],
            [5, -2, -4, 3],
        )
        for (n, _a, _b, expected) in data:
            with self.subTest(n=n, _a=_a, _b=_b, expected=expected):
                a = zp.Element(n, _a)
                b = zp.Element(n, _b)
                self.assertEqual((a * b).v, expected)
                self.assertEqual(a.v, _a % n)
                self.assertEqual(b.v, _b % n)

    def test_div(self):
        data = (
            [5, 0, 1, 0],
            [5, 1, 1, 1],
            [5, 1, 2, 3],
            [5, 1, 3, 2],
            [5, 1, 4, 4],
        )
        for (n, _a, _b, expected) in data:
            with self.subTest(n=n, _a=_a, _b=_b, expected=expected):
                a = zp.Element(n, _a)
                b = zp.Element(n, _b)
                self.assertEqual((a / b).v, expected)
                self.assertEqual(a.v, _a % n)
                self.assertEqual(b.v, _b % n)

    def test_div_zero(self):
        data = (
            [5, 1, 0, 0],
            [5, 1, 5, 1],
        )
        for (n, _a, _b, expected) in data:
            with self.subTest(n=n, _a=_a, _b=_b, expected=expected):
                a = zp.Element(n, _a)
                b = zp.Element(n, _b)
                with self.assertRaises(ZeroDivisionError):
                    self.assertEqual((a / b).v, expected)
                    self.assertEqual(a.v, _a % n)
                    self.assertEqual(b.v, _b % n)

    def test_neg(self):
        data = (
            [5, 0, 0],
            [5, 1, 4],
            [5, 2, 3],
            [5, 3, 2],
            [5, 4, 1],
            [5, 5, 0],
            [5, -1, 1],
        )
        for (n, _a, expected) in data:
            with self.subTest(n=n, _a=_a, expected=expected):
                a = zp.Element(n, _a)
                self.assertEqual((-a).v, expected)

    def test_pow(self):
        data = (
            [5, 1, 0, 1],
            [5, 1, 3, 1],
            [5, 2, 0, 1],
            [5, 2, 2, 4],
            [5, 2, 3, 3],
        )
        for (n, _a, _b, expected) in data:
            with self.subTest(n=n, _a=_a, _b=_b, expected=expected):
                a = zp.Element(n, _a)
                self.assertEqual((a ** _b).v, expected)
                self.assertEqual(a.v, _a % n)

if __name__ == '__main__':
    unittest.main()
