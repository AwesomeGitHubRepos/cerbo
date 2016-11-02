'''Convenient mathematical functions
'''

import decimal
import math
#import functools

from mython.compat import princ
#import mython.times

def gain(a,b):
    'Percentage gain: 100 * (a-b) / b'
    return 100.0 * (a-b) / b

# http://rosettacode.org/wiki/Averages/Median#Fortran
def median(aray):
    srtd = sorted(aray)
    alen = len(srtd)
    return 0.5*( srtd[(alen-1)//2] + srtd[alen//2])

def decn(x, n):
    "quantize a value X to N decimal places, as a decimal"
    q = decimal.Decimal("0.1") ** n
    return decimal.Decimal(x).quantize(q)

def dec2(x):
    "quantize X to 2 d.p."
    return decn(x, 2)


def identity(x): return x

def minlist(alist, key = identity):
    if len(alist) == 0: return None
    n0 = key(alist[0])
    v0 = alist[0]
    for i in range(1, len(alist)):
        if key(alist[i]) >= n0: continue
        n0 = key(alist[i])
        v0 = alist[i]
    return v0

def minlist_test():
    princ(minlist([0, 3, 2]))
    princ(minlist([4, 1, 3, 2]))
    princ(minlist([4, 1, 3, 2, -2]))
            
# http://stackoverflow.com/questions/8919718/financial-python-library-that-has-xirr-and-xnpv-function
def xirr(transactions):
    years = [(ta[0] - transactions[0][0]).days / 365.0 for ta in transactions]
    # years = [ta[0].days for ta in transactions]
    #year_min = minlist(years, key = times.days_since_epoch)
    #years = [ y - year_min for y in years]
    residual = 1
    step = 0.05
    guess = 0.05
    epsilon = 0.0001
    limit = 10000
    while abs(residual) > epsilon and limit > 0:
        limit -= 1
        residual = 0.0
        for i, ta in enumerate(transactions):
            residual += ta[1] / pow(guess, years[i])
        if abs(residual) > epsilon:
            if residual > 0:
                guess += step
            else:
                guess -= step
                step /= 2.0
    return guess-1

def xirr_test():
    from datetime import date
    tas = [ (date(2010, 12, 29), -10000),
            (date(2012, 1, 25), 20),
            (date(2012, 3, 8), 10100)]
    princ(xirr(tas)) #0.0100612640381
    tas = [ (date(2012, 1, 25), 20),
            (date(2010, 12, 29), -10000),            
            (date(2012, 3, 8), 10100)]
    princ(xirr(tas)) #0.0100612640381




# http://code.activestate.com/recipes/511478-finding-the-percentile-of-the-values/
def percentile(N, percent, key=lambda x:x):
    """
    Find the percentile of a list of values.

    @parameter N - is a list of values. Note N MUST BE already sorted.
    @parameter percent - a float value from 0.0 to 1.0.
    @parameter key - optional key function to compute value from each element of N.

    @return - the percentile of the values
    """
    if not N:
        return None
    k = (len(N)-1) * percent
    f = math.floor(k)
    c = math.ceil(k)
    if f == c:
        return key(N[int(k)])
    d0 = key(N[int(f)]) * (c-k)
    d1 = key(N[int(c)]) * (k-f)
    return d0+d1

# median is 50th percentile.
#median = functools.partial(percentile, percent=0.5)
