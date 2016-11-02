'''Example of using threads in python3

# example usage

import time


def myfunc(x):
    time.sleep(x)
    print("Hello from myfunc. Arg is", x)
    return x + 10


print(map_concurrently(myfunc, [1.5, 1.4, 1.6]))
    

'''

######################################################################
# generic code

import threading

class Strand(threading.Thread):

    def __init__(self, func, arg):
        threading.Thread.__init__(self)
        self.func = func
        self.arg = arg

    def run(self):
        self.result =  self.func(self.arg)

def map_concurrently(func, arg_list):
    strands = [Strand(func, el) for el in arg_list]
    for s in strands: s.start()
    for s in strands: s.join()
    results = [s.result for s in strands]
    return results



