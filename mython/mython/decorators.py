'''Sundry decorators'''

import functools

####################################################################
# memoize
# http://stackoverflow.com/questions/1988804/what-is-memoization-and-how-can-i-use-it-in-python
class Memoize:
    '''For example usage, see accts'''

    def __init__(self, f):
        self.f = f
        self.memo = {}
    def __call__(self, *args):
        if not args in self.memo:
            self.memo[args] = self.f(*args)
        return self.memo[args]


# http://wiki.python.org/moin/PythonDecoratorLibrary#Memoize
# added 23-Jun-2013
def memoize(obj):
    '''Example:
    def um_inc(x): print "called" ; return x +1
    inc = memoize(em_inc)
    inc(1) # prints "called", returns 2
    inc(2) # prints "called", returns 3
    inc(1) # returns 2 without printing
    '''
    

    cache = obj.cache = {}

    @functools.wraps(obj)
    def memoizer(*args, **kwargs):
        key = str(args) + str(kwargs)
        if key not in cache:
            cache[key] = obj(*args, **kwargs)
        return cache[key]
    return memoizer
