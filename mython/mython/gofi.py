#!/usr/bin/env python
# -*- coding: utf-8 -*-

""" gofi - google finance library

Some fields returned by Google:
c  - change in price from yesterday
cp - change in price from yesterday, in percentage
l  - latest price
t  - symbol (.e.g EZJ)
... others

"""
# 25-Nov-2012 Yahoo now considered too flakey - using Google Finance instead

########################################################################


#from __future__ import print_function


import datetime
import json
import optparse
import os.path
import pdb
import re
#import requests
import shlex
import string
import sys
import time
#import unicodedata
#import urllib
#import urllib2

import mython.compat
from mython.compat import princ
import mython.csvmc
import mython.multi_get
import mython.pytext

import mython.decorators

class Sym: pass

def fetch(url):
    return mython.compat.download_url(url)
    # return r.text

def convert_currency(currencyFrom, currencyTo):
    url = 'http://www.google.com/ig/calculator?hl=en&q=1' + currencyFrom + '%3D%3F' + currencyTo
    currencyline=fetch(url)
    result = re.search(".*rhs: \"(\d\.\d*)", currencyline)
    result = result.group(1)
    result = float(result)
    return result


def usd_to_gbp():
    result = convert_currency('USD', 'GBP')
    return result

usd_to_gbp = mython.decorators.Memoize(usd_to_gbp)

def get_url(ticker):
    return 'http://finance.google.com/finance/info?q=%s' % ticker

def unpack_response(lines):
    # http://coreygoldberg.blogspot.co.uk/2011/09/python-stock-quotes-from-google-finance.html
    try:
        lines = lines.decode("latin-1")
    except AttributeError:
        princ("ERR. lines is: ", lines)
        raise
    # princ(".",)
    lines = lines.split('\n')
    lines_string = [x.strip("\n") for x in lines]
    merged = ''.join([x for x in lines_string if x not in ('// [', ']')]) 
    vals1 = json.loads(merged)
    vals2 = { k : v.replace(",", "") for k, v in vals1.items() }
    return vals2

def download_all_fields(ticker):
    url = get_url(ticker)
    text = fetch(url)
    # princ("download_all_fields(): text:", text)
    return unpack_response(text)


def download_all_fields_async(tickers):
    requests = mython.multi_get.keyed_multi_get(tickers, get_url, timeout = 1.5)
    #for url, 

def extract_price(quote):
    price = quote['l']
    # price = ''.join([c for c in price if c in ".0123456789"])
    price = float(price)
    return price

def download_google(ticker_symbol):
    quote = download_all_fields(ticker_symbol)
    # princ("quote:", quote)
    return extract_price(quote)


def unitise_price(unit, price):
    if unit == 'P': # units are in pence
        price *= 0.01
    elif unit == 'USD': # units are in US dollars
        price *= usd_to_gbp()
    elif unit == 'GBP' or unit == 'NIL':
        pass # just keep the result
    else:
        fmt = 'Unrecognised unit. unit: "{0}", symbol: "{1}"'
        msg = fmt.format(unit, symbol)
        raise TypeError(msg)
    return price

def download_sym(unit, symbol):
    price = download_google(symbol)
    price = unitise_price(unit, price)
    return price

def download_syms_async(syms):
    threads = []
    for s in syms:
        thr = mython.multi_get.URLThread(get_url(s.gepic))
        s.thr = thr
        threads.append(thr)

    mython.multi_get.multi_get_threads(threads, timeout = 2.0)

    for s in syms:
        s.response = s.thr.response
        s.thr = None

def download_syms_sync(syms):
    for s in syms:
        princ("download_syms_sync():", s.gepic)
        s.response = download_all_fields(s.gepic)

def download_syms(syms, async = True):
    """Download multiple units and symbols"""
    if async:
        download_syms_async(syms)
    else:
        download_syms_sync(syms)


    for s in syms:
        #s.price = unitise_price(s.unit, extract_price(s.thr.response))
        r = s.response
        if r is None:
            princ("ERR: received no response for symbol: '" + s.gepic + "'")
            exit(1)
        r = unpack_response(r)
        s.quote = r
        p = extract_price(r)
        p = unitise_price(s.unit, p)
        s.price = p

                                  

    
########################################################################

def print_basic(quote):
    # print('Called basic')
    princ(quote['l'])

def print_perform(quote):
    "Verbose output"
    fields = [ ('t', 'Ticker'), ('l', 'Price'), ('c', 'Change (p)'), ('cp', 'Change (%)')]
    for f in fields:
        k, desc = f
        princ("{0:<10s} {1:>6s}".format(desc, quote[k]))


def print_commands(options, args):
    "Print options and args, then exit"
    princ("Options:", options)
    princ("Args:", args)
    sys.exit(0)

########################################################################

def today():
    "return the date to day in the form YYYY-MM-DD"
    d = datetime.date.today()
    return d.strftime('%Y-%m-%d')



def dateord(datestr):
    "convert YYYY-MM-DD into a date ordinal"
    y = int(datestr[0:4])
    m = int(datestr[5:7])
    d = int(datestr[8:10])
    return datetime.date(y,m,d).toordinal()

def ord0():
    "ordinal value for start of the year"
    t = datetime.date.today()
    y = t.year
    return datetime.date(y,1,1).toordinal()
    


def show(s, v):
	fmt = '%-5.5s %7.2f'
	txt = fmt % (s , v)
	princ(txt)

def percent(v): return (v-1.0)* 100.0

def show_relative(options, now):
    now = float(now)
    base = float(options.base)
    crr = now / base

    from_date = dateord(options.from_date)
    to_date = dateord(today())
    yearfrac = 365.0 / float(to_date - from_date)
    irr = pow(crr, yearfrac)
    show('START', base)
    show('NOW', now)
    show('INCR', now - base)
    show('CRR%', percent(crr))
    show('IRR%', percent(irr))    


########################################################################
def main():

    parser = optparse.OptionParser()
    parser.add_option("-a", "--all", action = "store_true", dest = "all",
                      help="download all in accts")
    parser.add_option("-b", "--base", dest="base", 
                      #default = 5412.88, # at 01-jan-2010
                      help="base value for comparison")
    parser.add_option("-c", "--commands", action="store_true", 
                      dest="commands", default = False,
                      help="Show command line arguments and exit")
    parser.add_option("-d", "--date", dest="from_date",
                      default= str(datetime.date(datetime.date.today().year, 1, 1)),
                      help="from date for comparison. Defaults to 1 Jan of current year")
    parser.add_option("-e", "--epic", dest="epic", 
                      help="EPIC symbol to download. Use ^FTSE for Footsie")
    parser.add_option("-o", "--oeic", dest="oeic",
                      help="Download an OEIC price")
    parser.add_option("-p", "--perform", action="store_false", 
                      dest='perform', default = True,
                      help='Show relative performance for year')
  
    (options, args) = parser.parse_args()
    if options.commands: print_commands(options, args)

    #debug = False ; #  debug = True # quickie debugging
    #if debug: # debugging purposes
    #    print("Options:", options)
    #    print("Args:", args)
    #    return

    #print options.base is None
    #return
    if options.all: download_all() ; return
    quote = None
    if options.epic is not None: quote = download_all_fields(options.epic)
    if options.oeic is not None: quote = download_oeic(options.oeic)

    #print(quote)
    #print(options.perform)
    if options.perform: print_basic(quote)
    else: print_perform(quote)
        
    #if options.base is None:
    #    print(quote)
    #else:
    #    show_relative(options, quote)

def mainXXX():
    download_all()

if __name__ == "__main__": main()


