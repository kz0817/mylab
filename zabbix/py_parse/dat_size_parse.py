#!/usr/bin/env python
import sys

entries = []
total_size = 0
for line in sys.stdin:
  words = line.split('\t')
  if len(words) != 2:
    continue 
  sizeStr = words[1].strip('\n')
  print sizeStr 
  if not sizeStr.isdigit():
    continue
  size = int(sizeStr)
  total_size += size
  elem = {'table':words[0], 'size':size}
  entries.append(elem)

# show ratio
for elem in entries:
  ratio = float(elem['size']) / total_size
  print "%.4f %s" % (ratio, elem['table'])
