#!/usr/bin/env python
import cPickle

def save(val):
    return cPickle.dumps(val, cPickle.HIGHEST_PROTOCOL)

def load(val):
    return cPickle.loads(val)

a = save(17)
print type(a)
print load(a)

b = save("foo")
print type(b)
print load(b)
