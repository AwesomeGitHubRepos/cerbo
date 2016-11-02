# http://www.artima.com/weblogs/viewpost.jsp?thread=240845

from functools import wraps

from mython.compat import princ

class Param(object):
    def __init__(self, default):
        self.default = default
        #self.func = func

    def __call__(self, func, *args):
        @wraps(func)
        def wrapper(*args):
            if len(args) >0:
                self.default = args[0]
            return self.default # self.func(*args)
        return wrapper




def example():
    @Param(42)
    def hi(newval):
        pass

    princ(hi()) # 42
    princ(hi(12)) # 12
    princ(hi()) # 12
    hi(13) # sets it to 13
    princ(hi()) # 13
