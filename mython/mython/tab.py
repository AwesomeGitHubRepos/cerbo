#!/usr/bin/env python

import argparse
#import shlex
import string
import sys

from mython.compat import princ
import mython.listmc

def pad(rows):
    trans = mython.listmc.transpose(rows)
    clens = [max(map(len, row)) for row in trans]

    result = []
    ncols = len(rows[0])
    for row in rows:
        cols = [row[c].rjust(clens[c]) for c in range(ncols)]
        #line = string.join(line, ' ')
        result.append(cols)
    return result


def astext(lol, padcols = True, delim = '  '):
    if padcols: lol = pad(lol)
    #lines = [ string.join(row, delim) for row in lol]
    lines = [ delim.join(row) for row in lol]
    text = '\n'.join(lines)
    # return string.join(lines, '\n')
    return text

def read(istream = sys.stdin):
    rows = []
    ncols = 0
    # lines = istream.readline()
    def gstr(): return istream.readline()
    def gint(): return int(gstr())
 
    nrows = gint()
    ncols = gint()

    rows = []
    for r in xrange(nrows):
        row = [gstr().strip() for c in xrange(ncols)]
        rows.append(row)
    return rows

def main(istream, args):
    row = read(istream)
    princ(astext(rows, args['pad'], args['delim']))


   
def test():
    textify([['as', 'b'], ['c', 'def']])
 
if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Create a table',
                                     add_help=True)
    parser.add_argument('-d', '--delim'  , # action='store', dest='delim',
                        default=' ', 
                        help='delimeter separating each column. Space default')
    parser.add_argument('-p', '--nopad',dest='pad', action="store_false",
                        help="Don't pad columns to equal width (default is to pad)")
    args = vars(parser.parse_args())

    main(sys.stdin, args)
