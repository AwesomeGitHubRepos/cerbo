import math

def nth(alist, n): return alist[n]
def nths(lol, n): return [x[n] for x in lol]

def first(alist): return nth(alist, 0)
def firsts(lol): return nths(lol, 0)
def seconds(lol): return nths(lol, 1)
def thirds(lol): return nths(lol, 2)

def uniq(seq):
    'Create a sublist of SEQ, removing duplicates, preserving order'
    result = []
    for el in seq:
        if el not in result: result.append(el)
    return result

def getkeys(seq, keyfunc):
    'Create a list of keys, preserving order'
    return uniq(map(keyfunc, seq))

def indexing(alist, start = 0):
    'Return an index number and elements of a list in turn'
    idx = start
    for el in alist:
        yield idx, el
        idx += 1

def revindex(alist):
    """Reverse index: Given ALIST, return a dictionary with elements of ALIST as keys
    and list index as values. E.g.
    listmc.revindex([ "a", "b", "c"]) => {'a': 0, 'b': 1, 'c': 2}"""
    #result = []
    return { k : v for v, k in indexing(alist) }
    
def takeby(n, alist, pad = True):
    """
    print takeby(2, [10, 11, 12, 13, 14]) => [[10, 11], [12, 13], [14, None]]
    print takeby(3, [10, 11, 12, 13], pad = False) => [[10, 11, 12], [13]]
    """
    result = []
    while len(alist) > 0:
        group = []
        for idx in range(n):
            try: 
                el = alist.pop(0)
            except IndexError:
                if not pad: continue
                el = None
            group.append(el)
        result.append(group)
    return result

def transpose(m):
    'transpose a list of lists'
    mout = []
    for c in range(len(m[0])):
        row = [m[r][c] for r in range(len(m))]
        mout.append(row)
    return mout

def buckets(n, alist):
    """Put elements of ALIST into N lists. Any 'spares' are put into last list
    buckets(5, [1,2,3,4,5,6,7,8,9,10])
    >>> [[1, 2], [3, 4], [5, 6], [7, 8], [9, 10]]
    buckets(3, [1,2,3,4,5,6,7,8,9,10])
    >>> [[1, 2, 3], [4, 5, 6], [7, 8, 9, 10]]
    """
    size = int(len(alist)/n)
    # print size
    remainder = alist
    result = []
    for r in range(n-1):
        front = remainder[0:size]
        remainder = remainder[size:]
        result.append(front)
    result.append(remainder)
    return result


def find_first(alist, targ, key = None):
    """Find first item in ALIST
    """
    for el in alist:
        if key is None: v = el
        else: v = key(el)
        if v == targ: return el
    return None
