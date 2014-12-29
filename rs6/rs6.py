"""
Find the top quintile for RS6

Assumes you have downloaded Sharelock query rs6 as StatsList.csv

The first column should contain rs6, and it is the first number in the output
"""
import os.path
import string

fp = file(os.path.expanduser('~/Downloads/StatsList.csv'), 'rbU')
lines = fp.readlines()

q1 = len(lines)/5
line = lines[q1]
print line