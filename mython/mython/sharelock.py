"""
Module for accessing Sharelock Holmes
"""

import argparse
import csv
#import http
#import io
import pprint
import re
import os
import os.path
#import shutil
#import subprocess
import sys

import mython.csvmc as csvmc
import mython.pytext as pytext

ROOT=os.getenv("HOME") + "/.fortran"
#MISC_DIR =  ROOT + "/int/misc"
#COOKIES_FILE = MISC_DIR + "/cookies.txt"
STATS_FILE = ROOT + "/StatsList.csv"

def fixfile(filename = STATS_FILE):
    txt01 = pytext.load(filename)
    txt02 = txt01.replace("\r", "\n")
    for (ori, rep) in [('F.EPIC', 'epic'), ('F.Sector', 'sector'),
                       ('MarketCap', 'mkt'), ('Piotroski_Score', "pio"), ('RS_5Year', 'rs5y'), ('RS_6Month', 'rs6mb'), ('RS_Year', 'rs1y')]:
        txt02 = txt02.replace(ori, rep, 1)
    rows01 = list(csv.reader(io.StringIO(txt02)))
    len0 = len(rows01[0])
    
    rows02 = []
    for r in rows01:
        cols = [c.strip() for c in r]
        rows02.append(cols[0:len0])

    csv.writer(open(filename, 'w')).writerows(rows02)
    return rows02


def momo():
    """Fix the standard sharelock file and create the momo.csv in the standard location
    """
    global STATS_FILE
    fixfile()
    d = csvmc.read_dict(STATS_FILE)
    def f(row):
        epic = row['epic']
        rs6mb = float(row['rs6mb'])
        rs1y  = float(row['rs1y'])
        rs6ma = (rs1y/100.0 + 1.0)/(rs6mb/100.0 + 1.0)*100.0 - 100.0
        rs6ma = "{:.2f}".format(rs6ma)
        res = { 'epic' : epic, 'rs6ma' : rs6ma, 'rs6mb' : row['rs6mb'], 'rs1y': row['rs1y']}
        return res

        
    d1 = []
    for r in d:
        try:
            rnew = f(r)
            d1.append(rnew)
        except ValueError:
            #print("Skipping:", rnew.epic)
            pass

    with open(ROOT + '/momo.csv', "w") as f:
        w = csv.DictWriter(f, ["epic", "rs6ma", "rs6mb", "rs1y"])
        w.writeheader()
        for r in d1: w.writerow(r)
        
    #print(d1)
        
    

    

def prdf(df):
    'print a data frame'
    print()
    print(df.to_string())

def read_csv():
    return csvmc.read_dict(ROOT + "/int/misc/calcs.csv")



############################################################################
# statistical percentiles for
# http://www.markcarter.me.uk/money/stats.htm

    """
Find the pernctiles
"""


import mython.maths

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

def mkt():    
    fname = os.path.expanduser('~/Downloads/AAA.csv')
    data = mython.csvmc.read_dict(fname)
    for k in ['PBV', 'PER']:
        prin_fstats(data, k, 10)

############################################################################        

cmd_help = """
Run a command:
momo - Fix the CSV file, and create ~/.fortran/momo.csv
"""
        
if __name__ == "__main__" :
    p = argparse.ArgumentParser()
    p.add_argument("--debug", action = 'store_true', help = "Print the arguments")
    p.add_argument("--momo", action = 'store_true', help = "Fix the CSV file, and create ~/.fortran/momo.csv")
    p.add_argument("--mkt", action = 'store_true', help = 'Run percentiles calc on market for http://www.markcarter.me.uk/money/stats.htm')
    #p.add_argument("cmd", help = cmd_help)
    args = p.parse_args()

    if args.debug: print(args)
    if args.mkt: mkt()
    if args.momo: momo()
    #print(args)
    print("Finished")
