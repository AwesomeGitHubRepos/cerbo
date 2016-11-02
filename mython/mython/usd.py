"""
Create USD conversion
"""

import datetime
import requests
import time
import warnings

#import pytext

def main():
    warnings.warn("fabo should be used for getting USD", DeprecationWarning)
    url = "http://download.finance.yahoo.com/d/quotes.csv?s=USDGBP=X&f=nl1d1t1"
    r = requests.get(url)
    text = r.text
    r.close()
    fields = text.split(",")
    rox = float(fields[1])* 100
    fmt = "P\t{0}\t{1}\tUSD\t{2:>7.2f}\tNIL\n"
    dt = datetime.datetime.now()
    dstamp = time.strftime("%Y-%m-%d")
    tstamp = time.strftime("%H:%M:%S")
    line = fmt.format(dstamp, tstamp, rox)
    #print(line)
    fname = "/home/mcarter/.ssa/gofi/USD.txt"
    try:
        fp = open(fname, "r")
        file_contents = fp.read()
        fp.close()
    except FileNotFoundError:
        file_contents = ""
    file_contents += line
    #print(file_contents)
    fp = open(fname, "w")
    fp.write(file_contents)
    fp.close()

if __name__ == "__main__":
    main()
