#!/usr/bin/env python

class Foo:
    def __init__(self):
        self._a = 5
        self.__b = 10

x = Foo()
print x._a
print x.__b

