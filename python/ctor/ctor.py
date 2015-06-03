#!/usr/bin/env python

class Base:
    def __init__(self):
        print "Base"

class Derived1(Base):
    pass

class Derived2(Base):
    def __init__(self):
        print "Derived2"

class Derived3(Base):
    def __init__(self):
        Base.__init__(self)
        print "Derived2"


print "--"
d1 = Derived1()
print "--"
d2 = Derived2()
print "--"
d3 = Derived3()
