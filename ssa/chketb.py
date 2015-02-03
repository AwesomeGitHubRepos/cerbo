"""
Check that the number in the ETB files add up
"""

from decimal import *
import glob
import shlex

def checkfn(fn):

    txt = open(fn).readlines()
    total = Decimal('0.00')
    for idx, line in enumerate(txt):
        line = line.strip()
        if len(line) <2: continue
        if line[0] != '2': continue
        line = line.replace(',', '')

        def oops(): return 'Error in {0}:{1}: <{2}>'.format(fn, idx+1, line)

        try:
            fields = shlex.split(line)
        except:
            #print('Error in {0}:{1}: <{2}>'.format(fn, idx+1, line))
            print(oops())
            raise
        cur = Decimal(fields[-2])
        tot = Decimal(fields[-1])
        total += cur
        if total != tot:

            raise ArithmeticError(oops())
        #print(fields)
        #print(cur, tot)
    #print(txt)

def main():
    for fn in glob.glob("/home/mcarter/.ssa/etb/*.txt"):
        checkfn(fn)
    print('Finished. All correct')

if __name__ == "__main__":
    main()
