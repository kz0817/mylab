#!/usr/bin/env python
import sys

entries = []
total_size = 0;
for line in sys.stdin:
    words = line.split('\t')
    if len(words) != 2:
        continue
    sizeStr = words[1].strip('\n')
    if not sizeStr.isdigit():
        continue
    size = float(sizeStr)
    elem = {'table':words[0], 'size':size}
    entries.append(elem)
    total_size += size

print "Total: %d" % total_size
print "Total: %.2f (MB)" % (total_size / 1024 /1024)
for elem in entries:
    print "%02.2f %s" % (elem['size']/total_size*100, elem['table'])



