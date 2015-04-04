#!/usr/bin/env python3
"""
Find the medians by a group
"""

import itertools
import os
from os.path import isfile

import mython
import mython.csvmc
import mython.maths
#import csvmc
#import maths

#fname1 = r'C:\Users\mcarter\Downloads\StatsList.csv'
fname1 = '/cygdrive/c/Users/mcarter/Downloads/StatsList.csv'
if isfile(fname1):
	fname = fname1
else:
	fname = os.path.expanduser('~/Downloads/StatsList.csv')

data_raw = mython.csvmc.read_dict(os.path.expanduser(fname))

#def keyfunc(x): return x['FTSE_Index']
#def keyfunc(x): return x['Sub_Sector']
#def keyfunc(x): return x['Sector']
#data = sorted(data, key=keyfunc)
#field = 'PER'
#field = 'EV_Sales'

#def by_groupfunc(fp, fieldname, keyfunc):
	
def report(grp, stat, raw_data):
	print("\n\n--- GROUP:", grp, ", STAT:", stat, " ---")
	def keyfunc(x): return x[grp]
	data = sorted(raw_data, key = keyfunc)
	for k, g in itertools.groupby(data, key = keyfunc):
		els = [ el[stat] for el in g]
		floats = [float(el) for el in els if len(el)>0]
		if len(floats)>0:
			m = "{0:7.2f}".format(mython.maths.median(floats))
		else:
			m = "NO DATA"
		print("{0:<22.22} {1}".format(k, m))

stats = ['PER', 'EV_Sales', 'PBV', 'ROE'] 
groups = ['FTSE_Index', 'sector', 'Sub_Sector']
#for fn in ['PER', 'EV_Sales', 'FTSE_Index']: by_groupfunc(fn, keyfunc)
#print data[1]
for g, s in itertools.product(groups, stats): report(g, s, data_raw)
