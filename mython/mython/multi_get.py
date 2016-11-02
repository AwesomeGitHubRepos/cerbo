"""
http://lethain.com/parallel-http-requests-in-python/
"""

from threading import Thread, enumerate
#from urllib2 import urlopen
from time import sleep

import mython.compat

UPDATE_INTERVAL = 0.01

class URLThread(Thread):
    def __init__(self,url):
        super(URLThread, self).__init__()
        super(URLThread, self).setDaemon(True)
        # self.key = key
        self.url = url
        self.response = None

    def run(self):
        #self.request = urlopen(self.url)
        #self.response = self.request.read()
        self.response = mython.compat.download_url(self.url)

def multi_get_threads(threads,timeout=2.0):
    def alive_count(lst):
        #alive = map(lambda x : 1 if x.isAlive() else 0, lst)
        #return reduce(lambda a,b : a + b, alive)
        alive = 0
        for x in lst: 
            if x.isAlive(): alive +=1
        return alive
    for thread in threads:
        thread.start()
    while alive_count(threads) > 0 and timeout > 0.0:
        timeout = timeout - UPDATE_INTERVAL
        sleep(UPDATE_INTERVAL)

def multi_get(uris,timeout=2.0): 
    threads = [ URLThread(uri) for uri in uris ]
    multi_get_threads(threads)
    return [ (x.url, x.response) for x in threads ]

def keyed_multi_get(keys,make_uri, timeout=2.0):
    threads = [ URLThread(make_uri(k), k) for k in keys ]
    multi_get_threads(threads)
    dic = {x.key:x.response  for x in threads }
    return dic
