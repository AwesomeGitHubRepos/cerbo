"Mark Carters CSV functions"

import argparse
import csv
import os.path
import sys

from mython.compat import princ
import mython.listmc
import mython.tab as tab


def read_dict(filename):
    "Read a CSV FILENAME as a list of dictionaries"
    filename = os.path.expanduser(filename)
    results = []
    with open(filename, "rU") as csvfile:
        reader = csv.DictReader(csvfile)
        for row in reader:
            #if len(row) == 0 and skip_empty: continue
            results.append(row)
    return results

def read_csv(filename, skip_empty = False):
    "Read a filename into a list of lists"
    filename = os.path.expanduser(filename)
    results = []
    with open(filename, "rU") as csvfile:
        reader = csv.reader(csvfile)
        for row in reader:
            if len(row) == 0 and skip_empty: continue
            results.append(row)
    return results
    
def write_csv(lol, filename):
    "Write list-of-list LOL to FILENAME"
    filename = os.path.expanduser(filename)

    # http://stackoverflow.com/questions/7200606/python3-writing-csv-files
    if sys.version_info >= (3,0,0):
        f = open(filename, 'w', newline='')
    else:
        f = open(filename, 'wb')

    writer = csv.writer(f)
    for row in lol: writer.writerow(row)
    f.close()

def write_dict(alod, filename):
    "Write list-of-dictts ALOD to FILENAME"
    filename = os.path.expanduser(filename)
    row0 = alod[0]
    with open(filename, "wb") as csvfile:
        writer = csv.DictWriter(csvfile, row0.keys())
        writer.writeheader()
        for row in alod:
            writer.writerow(row)


def keyval(filename, keyfield, keyval, valfield):
    """Given a csv FILENAME, the name of a KEYFIELD, and its KEYVAL, return the
    first value corresponding to VALFIELD. E.g. given a file foo.csv:
    a,b,c
    1,2,3
    4,5,6
    keyval("a", 4, "b", "foo.csv" => 5"""
    data = read_csv(filename)
    idxs = listmc.revindex(data[0])
    keynum = idxs[keyfield]
    valnum = idxs[valfield]
    data = data[1:]
    for row in data:
        if row[keynum] == keyval: return row[valnum]
    return None # failure
    
def astext(filename):
    c = read_csv(filename, True)
    princ(tab.astext(c))

# def stdin_to_ascii():
def tab_to_csv():
    lol = tab.read(sys.stdin)
    writer = csv.writer(sys.stdout)
    for row in lol:
        writer.writerow(row)

def main():
    parser = argparse.ArgumentParser(description='CSV processing')
    parser.add_argument('-c', '--commands', dest = 'commands', 
                        default = False,
                        action= "store_true",
                        help = 'Print commands and exit')

    parser.add_argument('--from-tab', 
                        action= "store_true",
                        help = 'convert to csv format from tab-format')
    parser.add_argument('-a', '--ascii', action = 'store_true', 
                        help = 'convert to ASCII format')
    parser.add_argument("file")

           
    args = parser.parse_args()   

    if args.commands: princ(args) ; exit(0)
    if args.from_tab: tab_to_csv()
    if args.ascii is not None: astext(args.file)


if __name__ == "__main__": main()
    
