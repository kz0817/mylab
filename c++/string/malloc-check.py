#!/user/bin/env python

class MyBreakpoint(gdb.Breakpoint):
  def __init__(self, *args, **kwargs):
    gdb.Breakpoint.__init__(self, *args, **kwargs)

  def stop(self):
    gdb.execute("bt")
    return False

gdb.execute("delete")
gdb.execute("start")
bp = MyBreakpoint("malloc")
gdb.execute("continue")
