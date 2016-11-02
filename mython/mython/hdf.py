"""
Library for converting to/from HDF5
"""

import argparse
import datetime
import os.path

import h5py
import numpy as np
import pandas as pd

import mython.quant

def to5(filename):
    """Convert a CSV file to an HDF5 file. It is fairly intelligent about dates and strings.
    Dates will be expressed as floats of the form YYYYMMDD

    Pandas has trouble distinguishing between dates and strings.
    """
    
    df = mython.quant.load_to_pandas(filename)
    try:
        h5 =h5py.File('/home/mcarter/.fortran/' + filename + '.h5')
        h5.attrs['Created'] = str(datetime.datetime.now())
        for cname in df.columns:
            arr = np.array(df[cname])
            atts = None
            #if df[cname].dtype == datetime.date:
            #    def d2f(x): [y, m, d] = map(int, str(x).split('-')) ; return y *10000 + m * 100 + d
            #    arr = np.array([d2f(x) for x in arr], dtype = 'float')
            #    atts = { 'NB': "In float form for octave compatability", 'Format' : 'YYYYMMDD' }
            if arr.dtype == 'O': # probably a string
                mlen = max([len(str(a)) for a in arr])
                arr = arr.astype(dtype = "S" + str(mlen))

            dset = h5.create_dataset(cname, data = arr, compression='gzip')
            if atts:
                for k, v in atts.items(): dset.attrs[k] = v
    finally:    
        h5.close()


def get_dset(fname, dset):
    h5 = h5py.File(fname, 'r')
    for x in h5[dset]: print(x)
    h5.close()
    
if __name__ == "__main__":
    p = argparse.ArgumentParser()
    p.add_argument('--debug', action = "store_true", help = "Print the arguments")
    p.add_argument('--input', action = "store", help = "Input filename")
    p.add_argument('--get-dset', action = "store", help = "Get and print a dataset, specifiying the path if necessary")
    p.add_argument('--to5', action = "store", help = "Convert csv file to HDF5. File must be in ~/.fortran directory")

    args = p.parse_args()
    if args.debug: print(args)
    if args.get_dset: get_dset(args.input, args.get_dset)
    if args.to5: to5(args.to5)
