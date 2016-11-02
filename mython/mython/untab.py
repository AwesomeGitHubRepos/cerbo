#!/usr/bin/env python

import shlex
import sys

def main(istream= sys.stdin):
    rows = []
    ncols = 0
    lines = istream.readlines()
    for line in lines:
        line = line.strip('\n')
        if len(line) == 0: continue
        if line[0] == '#': continue
        row = shlex.split(line)
        if len(row) == 0: continue
        #print row
        ncols = max(ncols, len(row))
        rows.append(row)

    print(len(rows))
    print(ncols)
    for row in rows:
        for col in row:
            print(col)


    
if __name__ == "__main__":
    # istream = sys.stdin
    main()
