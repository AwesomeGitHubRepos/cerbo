'''Pickle convenience utilities'''

import pickle

def dump(obj, filename):
    'Dump object OBJ to file FILENAME'
    with open(filename, "wb") as f:
        pickle.dump(obj, f)

def load(filename):
    'Load an objected from file FILENAME'
    with open(filename, "rb") as f:
        return pickle.load(f)
