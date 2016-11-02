"""
Module for accessing Sharelock Holmes
"""

import argparse
#import cookielib
import csv
import http
import io
import pprint
import re
import os
import os.path
import shutil
import subprocess
import sys

#import mechanize

import mython.csvmc as csvmc

# import pandas # apt-installable

#sys.path.append('/home/mcarter/repos/nokilli/python')

import mython.pytext as pytext

#ROOT = os.getenv("DOCS") + "/sharelock"
ROOT=os.getenv("HOME") + "/.fortran"
MISC_DIR =  ROOT + "/int/misc"
COOKIES_FILE = MISC_DIR + "/cookies.txt"
#STATS_FILE = MISC_DIR + "/StatsList.csv"
STATS_FILE = ROOT + "/StatsList.csv"

def fixfile(filename = STATS_FILE):
    txt01 = pytext.load(filename)
    txt02 = txt01.replace("\r", "\n")
    for (ori, rep) in [('F.EPIC', 'epic'), ('F.Sector', 'sector'),
                       ('MarketCap', 'mkt'), ('Piotroski_Score', "pio"), ('RS_5Year', 'rs5y'), ('RS_6Month', 'rs6mb'), ('RS_Year', 'rs1y')]:
        txt02 = txt02.replace(ori, rep, 1)
    #txt03 = txt02.replace(' "', '"')
    #txt03 = re.sub(r',$', '', txt02)
    
    #rdr = csv.reader(io.StringIO(txt02))
    #hdr = rdr.next()
    #print(hdr)
                     
    #txt3 = ''                
    rows01 = list(csv.reader(io.StringIO(txt02)))
    len0 = len(rows01[0])
    
    rows02 = []
    for r in rows01:
        cols = [c.strip() for c in r]
        rows02.append(cols[0:len0])
    #rows02 = [row[0:len0] for row in rows01]
    #rows02 = rows01+body

    csv.writer(open(filename, 'w')).writerows(rows02)
    return rows02

    

def csvnameXXX():
    "Return the full path of the latest downloaded StatList*.csv file"
    cmd = "ls -t ~/Downloads/StatsList*.csv | head -1"
    filein = subprocess.check_output(cmd, shell=True)
    filein = filein.strip()
    filein = os.path.expanduser(filein)
    return filein


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

    #csvmc.write_dict(d1, ROOT + '/momo.csv')
    with open(ROOT + '/momo.csv', "w") as f:
        w = csv.DictWriter(f, ["epic", "rs6ma", "rs6mb", "rs1y"])
        w.writeheader()
        for r in d1: w.writerow(r)
        
    #print(d1)
        
    

    
CLEAN_STATSLISTXXX = ROOT + "/int/misc/clean-statslist.csv"
def clean_latest_statslist():
    "Identify latest StatsList file, and clean it up"
    global CLEAN_STATSLISTXXX
    fixfile(csvname)
    shutil.copyfile(filein, CLEAN_STATSLIST)
    



def loadclean():
    global CLEAN_STATSLIST
    clean_latest_statslist()
    return pandas.read_csv(CLEAN_STATSLIST)

def prdf(df):
    'print a data frame'
    print()
    print(df.to_string())

def read_calcsXXX():
    "Read the calculations csv file as a pandas dataframe"
    global ROOT
    return pandas.read_csv(ROOT + "/int/misc/calcs.csv")

def read_csv():
    return csvmc.read_dict(ROOT + "/int/misc/calcs.csv")

############################################################################
# automation of the rs6 file

def print_forms(br):
    "Useful for debugging purposes"
    for form in br.forms():
        print(form)
        print("--------------------------------")

def print_links(br):
    "Useful for debugging purposes"
    for link in br.links():
        # print link
        # Link(base_url='http://www.example.com/', url='http://www.rfc-editor.org/rfc/rfc2606.txt', text='RFC 2606', tag='a', attrs=[('href', 'http://www.rfc-editor.org/rfc/rfc2606.txt')])
        # print link.url
        # print
        pass


def get_stats(query_name):
    """Fetch rs6 from Sharelock, and save it in a canonical place
    You may have to periodically update your cookies
    """
    global COOKIES_FILE, STATS_FILE
    br = mechanize.Browser()
    br.set_handle_equiv(True)
    #br.set_handle_gzip(True)
    br.set_handle_redirect(True)
    br.set_handle_referer(True)
    br.set_handle_robots(False)
    br.set_handle_refresh(mechanize._http.HTTPRefreshProcessor(), max_time=1)
    br.addheaders = [('User-agent', 'Mozilla/5.0 (X11; U; Linux i686; en-US; rv:1.9.0.1) Gecko/2008071615 Fedora/3.0.1-1.fc9 Firefox/3.0.1')]
    cj = cookielib.MozillaCookieJar()
    br.set_cookiejar(cj)
    cj.load(COOKIES_FILE, ignore_discard=False, ignore_expires=False)

    #r = br.open('http://www.sharelockholmes.com/StatsList.aspx')
    br.open('http://www.sharelockholmes.com/')
    br.follow_link(url='StatsList.aspx')
    br.select_form(name="Form1")
    # br.select_form(name="Fc")
    control=br.form.find_control(name="cmbQuery")
    control.value = [query_name]
    br.submit(name="cmdExport")

    output = br.response().read()
    open(STATS_FILE, "w").write(output)
    fixfile(STATS_FILE)


def get_rs6(): get_stats("rs6")

############################################################################

def main_a():   
    df = loadclean()
    print(df) # print descriptive info
    #df = df[(df.EV_EBITDA <=6.5) & (df.Yield >= 4.0)] # this works
    df = df[df.EV_EBITDA <=6.5]

    df = df[['EPIC', 'EV_EBITDA', 'Yield']]
    df = df.sort_index(by='EV_EBITDA')

    df.index = range(1, 1+ len(df)) # renumber the rows sequentially
    print(df.to_string())
    print("Number of rows:", len(df))

    print('Finished')

############################################################################
# statistical percentiles for
# http://www.markcarter.me.uk/money/stats.htm

    """
Find the pernctiles
"""


#import mython.csvmc
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

def mkt():
	data = mython.csvmc.read_dict(os.path.expanduser('~/.fortran/StatsList.csv'))
	#print("NUM=", len(data))
	for k in ['mkt', 'rs6mb', 'PBV', 'PER']:
		prin_fstats(data, k, 10)

#if __name__ == "__main__":
#	main()

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
