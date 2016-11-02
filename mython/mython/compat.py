# -*-coding: utf-8 -*-
# version 2/3 compatability layer
from __future__ import print_function

import platform
__vers = platform.python_version()

import io
import pdb
import traceback

def is3():
    "Determines if we are using version 3 of Python"
    return __vers[0] == "3"

def is2(): return __vers[0] == "2"

#if is2(): from __future__ import print_function

##########################################################################################

def princ(*args):
    # traceback.print_stack()
    print(*args)
    # pdb.set_trace()


def ascii(text):
    """Definitely convert an input to ASCII"""
    return ascii3(text) if is3() else ascii2(text)

def ascii2(text):
    import unicodedata
    text = unicode(text, 'Latin-1')
    text = unicodedata.normalize('NFKD', text).encode('ascii','replace')
    return text

def ascii3(text):
    #text = text.encode("ascii", "replace")
    #text = text.replace("£", "")
    text = str(text)
    #princ(text)
    return text

def download_url(url):
    return download_url3a(url) # if is3() else download_url2(url)

def download_url_lines(url):
    # python 3 way
    import requests
    r = requests.get(url)
    file_like_obj = io.StringIO(r.text)
    lines = file_like_obj.readlines()
    return lines


def download_url2(url):
    import urllib
    f = urllib.urlopen(url, proxies={})
    text = f.read()
    f.close()
    return text

def download_url3(url):
    import http.client
    import urllib.parse
    u = urllib.parse.urlparse(url)
    conn = http.client.HTTPConnection(u.netloc)
    conn.request("GET", u.path)
    resp = conn.getresponse()
    result = resp.read()
    conn.close()    
    return result

def download_url3a(url):
    import requests
    r = requests.get(url)
    text = r.content
    # princ(text)
    return text # r.text

#def izip    
#>>> urlparse('www.cwi.nl:80/%7Eguido/Python.html')
#ParseResult(scheme='', netloc='', path='www.cwi.nl:80/%7Eguido/Python.html',
#           params='', query='', fragment='')

#>>> from urllib.parse import urlparse
#>>> o = urlparse('http://www.cwi.nl:80/%7Eguido/Python.html')
#>>> o   
#ParseResult(scheme='http', netloc='www.cwi.nl:80', path='/%7Eguido/Python.html',
#            params='', query='', fragment='')
