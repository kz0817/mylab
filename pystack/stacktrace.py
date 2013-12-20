#!/usr/bin/env python
import traceback

def func2():
    print "foo"
    #print repr(traceback.extract_stack())
    #print repr(traceback.format_stack())
    print traceback.format_stack()

def func1():
    func2()

func1()
