"""Module to put the data into an HDF5 file
"""

import os
import shlex
from os import listdir
from os.path import isfile, join

import h5py
import numpy as np

import mython.listmc


# converter routines

def cstr(x): return str(x)

def cdate(x):
    #print(x.split("-"))
    y, m, d = map(int, x.split("-"))
    return (y * 100 + m) * 100 + d

def cpennies(x): return int(round(float(x) *100, 0))
    
#cdate("2014-11-12")
#cpennies(-123.999999)


def echo(args):
    print(args[0])

def parsedir(args):
    d = args[0]
    #print("TODO: parsedir", str(d))
    for f in listdir(d):
        full = join(d, f)
        if not isfile(full): continue
        cmdloop(full)
        

def add_to_class(cls, args):
    if not 'data' in cls.__dict__:
        #cls['data'] = []
        #cls.__dict__.data = []
        cls.data = []
        
    cls.data.append(args)

   
class comms: pass
def comm(args): add_to_class(comms, args)
    
#class descs: pass
def desc(args):
    class_name = args[0] + "s"
    cls = eval(class_name)
    if not 'desc' in cls.__dict__: cls.desc = []
    #print("Adding desc:", args)
    cls.desc.append(args)
    #add_to_class(descs, args)
    
class returns: pass
def do_return(args): add_to_class(returns, args)
    
class etrans: pass
def etran(args): add_to_class(etrans, args)
    
class fins: pass
def fin(args): 
    #print("Adding:", args)
    add_to_class(fins, args)
    
class naccs: pass
def nacc(args): add_to_class(naccs, args)
    
class ntrans: pass
def ntran(args): add_to_class(ntrans, args)
    
class naccs: pass
def nacc(args): add_to_class(naccs, args)
    
class prices: pass
def P(args): add_to_class(prices, args)


    
class periods: pass
def period(args): add_to_class(periods, args)
        
def cmdloop(filename):
    filename = os.path.expanduser(filename)
    for line in open(filename).readlines():
        sh = shlex.split(line, comments = True)
        if len(sh) == 0: continue
        #cmd, args = sh
        cmd = sh[0]
        if cmd == "return": cmd = "do_return"
            
        cmd = eval(cmd)
        cmd(sh[1:])
        #print(cmd, args)
        



def create_group(h5, cls):
    grp = h5.create_group(cls.__name__)
    for desc, v in zip(cls.desc, mython.listmc.transpose(cls.data)):
        cmd, name, dclass, dlen, conv = desc
        conv = eval(conv)
        
        data = []
        for d in v:
            try:
                data.append(conv(d))
            except ValueError:
                print("Class =", cls, ", converter=", conv, ", data =", d)
                raise
        #data = [conv(d) for d in v]
        dtype = dclass + dlen
        data = np.array(data, dtype = dtype)
        grp[name] = data # .create_dataset(name, shape=(dlen,), data = data, dtype = dtype, compression = "gzip")
        #print(data)
    
def write_hdf():
    try:
        h5 = h5py.File("/home/mcarter/.fortran/ssa.h5", "w")
        for cls in [comms, etrans, fins, naccs, ntrans, prices, returns]:
            create_group(h5, cls)

        for i, attr in zip(range(2), ["start", "end"]):
            h5['/'].attrs.modify(attr, cdate(periods.data[0][i]))
    finally:    
        h5.close()

def main():
    print("Module mython.fabo.hdf called")
    cmdloop("~/.ssarc")
    write_hdf()

