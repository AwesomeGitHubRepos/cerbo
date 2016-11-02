#import array
from collections import namedtuple
import csv
from pprint import pprint 
import sys

import h5py
import numpy as np

import mython.listmc

Descriptor = namedtuple('Descriptor', ['reverse', 'columns'])

yahoo = Descriptor(True, [['Close', 'closing', float, "float64"],
                          ['Date',  'dstamp',  str,   "S10"],
                          ['Volume', 'vol',    float, "float64"]])

def write_h5(outfile, group, arrdict):
    try:
        h5 = h5py.File(outfile)
        grp = h5.require_group(group)
        for dst_name, arr in arrdict.items():
            grp.create_dataset(dst_name, data = arr, compression="gzip")
    finally:
        h5.close()
    #print(arr)

def make(infile, src_field, dst_name, group, dtype, outfile, reverse = False):
    pass

def read_csv(csv_filename, reverse = False):
    with open(csv_filename) as c:
        rdr = csv.reader(c)
        hdr = next(rdr)
        data = [r for r in rdr]

    if reverse: data.reverse()
    datat = mython.listmc.transpose(data)
     
    datadic = {}
    for name, values in zip(hdr, datat): datadic[name] = values
    return datadic

def make_arrays(data, cols):
    nparrays = {}
    for col in cols:
        src, dst, former, dtype = col
        row = [ former(x) for x in data[src]]
        #if dst== "dstamp":
        #    arr = np.array(row, dtype = "S10", order = 'F')
        #    print(arr)
        #else:
        arr = np.array(row, dtype = dtype)
        nparrays[dst] = arr
    return nparrays

def main():
    sym = sys.argv[1]
    fin = sym.upper()
    fout = sym.lower()
    fout = fout.replace("^", "")
    fout = fout.replace(".", "_")
    fout += ".h5"

    rev, cols = yahoo
    data = read_csv(fin, rev)
    nparrays = make_arrays(data, cols)
    write_h5(fout, "/", nparrays)


if __name__ == "__main__":
    main()
