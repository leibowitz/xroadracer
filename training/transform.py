#!/bin/python
import sys
if len(sys.argv) < 2:
    sys.exit(1)
idx = int(sys.argv[1])

with open('map1.txt', 'r') as fp:
    i = 0
    while(1):
        line = fp.readline()
        #print('input-%d %s' %( i, line)),
        print(line)
        i += 1
        if not line:
            break
        # skip
        line = fp.readline()
        if not line:
            break
        
        line = fp.readline()
        #print('output-%d %s' %( i, line)),
        i += 1
        if not line:
            break

        output = line.split()
        
        print("%s\n" % output[idx])

        # skip
        line = fp.readline()
        if not line:
            break

    fp.close()

