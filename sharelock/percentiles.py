"""
Find the pernctiles
"""

#import itertools
import os

#import scipy
#import scipy.stats

import mython.csvmc
import mython.maths



#def keyfunc(x): return x['FTSE_Index']
#def keyfunc(x): return x['RS_6Month']
#def keyfunc(x): return x['EV_Sales']

def get_floats(data, fieldname):
	floats = []
	for el in data:
		f =  el[fieldname]
		try:
			f = float(f)
		except ValueError:
			#print "Skipping :*" + f + "*"
			continue
		floats.append(f)
	floats.sort()
	return floats

#print f

# [float(keyfunc(el)) for el in data if len(el)>0]

#floats.sort()
#print floats

def prin_fstats(data, fieldname, step):
	print(fieldname)
	floats = get_floats(data, fieldname)
	print("NUM=", len(floats))

	rng = range(0, 100 + step, step)
	for pc in rng:
		# score = scipy.stats.scoreatpercentile(floats, pc)
		score = mython.maths.percentile(floats , pc/100)
		print("{0:02d}% {1:8.2f}".format(pc, score))
	print()

def main():
	data = mython.csvmc.read_dict(os.path.expanduser('~/Downloads/StatsList.csv'))
	#print("NUM=", len(data))
	for k in ['MarketCap', 'RS_6Month', 'PBV', 'PER']:
		prin_fstats(data, k, 10)

if __name__ == "__main__":
	main()
