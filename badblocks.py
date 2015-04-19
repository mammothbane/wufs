#!/usr/bin/python
import sys

start = 33
end = 430

if sys.argv[1] is not '':
    start = int(sys.argv[1])
if sys.argv[2] is not '':
    end = int(sys.argv[2])

if not start < end:
    raise Exception('invalid args (start must be less than end)')

for i in range(start, end):
   print(i)
