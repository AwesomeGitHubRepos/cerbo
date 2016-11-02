import datetime
import functools
import json
import requests

import prettytable
import ystockquote

import mython.multi

#convert usd to gbp
# https://github.com/scottjbarr/yahoo_currency
def usd():
    url = "http://download.finance.yahoo.com/d/quotes.csv?s=USDGBP=X&f=nl1d1t1"
    resp = requests.get(url).content
    fields = resp.decode('utf-8').split(",")
    rox = 100 * float(fields[1])
    return rox

def getvals(comm):
    yticker = comm['yticker']
    d = ystockquote.get_all(yticker)
    res = {'yahoo' : yticker, 'price' : d['price'], 'change' : d['change'] }
    return res

# augment comms
def aug(comm, prices, changes, rox):
    sym = comm['sym']
    price = prices[comm['yticker']]
    #if comm['unit'] == "GBP": price = price * 100
    if comm['unit'] == "USD": price = price * rox
    comm['price'] = price
    comm['change'] = changes[comm['yticker']]
    if comm['unit'] == "USD": comm['change'] *=  rox
    comm['chgpc'] = 100* price/ (price-comm['change']) -100
    comm['day_profit'] = comm['change'] * comm['eqty']/100
    return comm

def wout(fp, dstamp, tstamp, cmd, sym, val, dtype):
    val = "{0:12.4f}".format(val)
    f = [cmd, dstamp, tstamp, sym, val, "P"]
    txt = '\t'.join(f) + "\n"
    fp.write(txt)  


def main():
    j = json.loads(open("/home/mcarter/.ssa/comm.json").read())
    yj = [comm for comm in j if comm['yticker'] != "."]
    yvalues = mython.multi.map_concurrently(getvals, yj)
    changes = { y['yahoo'] : float(y['change']) for y in yvalues }
    prices = { y['yahoo'] : float(y['price']) for y in yvalues }

    rox=usd()

    yj_aug = [ aug(comm, prices, changes, rox) for comm in yj]


    now = datetime.datetime.now()
    dstamp = now.strftime("%Y-%m-%d")
    tstamp = now.strftime("%H:%M:%S")


    fname = "/home/mcarter/.ssa/gofi/" + dstamp + ".txt"
    fp = open(fname, "w")
    out = functools.partial(wout, fp, dstamp, tstamp)
    out("fx", "USD", rox, "P")
    for comm in yj_aug:
        out("P", comm['sym'], comm['price'], "P")
    #out("x", "y", "z")
    fp.close()
    #dstamp, tstamp

    # create report
    profit = 0
    tvalue = 0

    pt = prettytable.PrettyTable(['Sym', 'Value', 'Profit', 'Chg%'])
    pt.align = "r"
    pt.float_format = ".2"
    share_tickers = [comm for comm in yj_aug if comm['type'] == "Y"]
    for comm in share_tickers:
        dprofit = comm['day_profit']        
        profit += dprofit
        value = comm['eqty'] * comm['price']/100
        tvalue += value
        fmt = "{0:>7} {1:>9.2f} {2:>7.2f} {3:7.2f}"
        #txt = fmt.format(comm['yticker'], value, dprofit, comm['chgpc'])
        pt.add_row([comm['yticker'], value, dprofit, comm['chgpc']])
        #print(txt)



    gainpc = 100* profit/(tvalue-profit)
    pt.add_row(['TOTAL', tvalue, profit, gainpc])


    indices = [comm for comm in yj_aug if comm['type'] == "I"]
    for comm in indices:
        pt.add_row([comm['yticker'], comm['price'], comm['change'], comm['chgpc']])

    print(pt)

if __name__ =="__main__":
    main()
