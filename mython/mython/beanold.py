"""
Interface to the Linux beancounter application using postgres as
a backend

I'm now no longer using that method, so I can probably ignore
this module. Superceded by module 'quant'.
"""

import atexit

import psycopg2

import matplotlib.pyplot as plt

import mython.listmc

db = None

def close_db():
    global db
    if db is None: return
    print("Closing database")    
    db.close()
    db = None

def open_db(force = False):
    """Open the database, with autoclose at exit"""
    global db
    if db is not None and not force:
        print("db is apparently already open. Maybe try force")
        return
    connstr = "dbname = 'beancounter'"
    db = psycopg2.connect(connstr)
    atexit.register(close_db, db)
    return db



def main():
    db = bean.open_db()
    cur = db.cursor()
    sql = "select date, day_close, volume from stockprices where symbol='HYH'"
    cur.execute(sql)
    rows = cur.fetchall()
    db.close()
    #print(rows)
    x = mython.listmc.firsts(rows)
    x = [ (d - date(2014, 11, 3)).days for d in x]
    print(x)
    y = mython.listmc.seconds(rows)
    p1 = plt.subplot(211)
    p1.set_title("HYH Share price ($)")
    p1.plot(x, y)

    vols = mython.listmc.thirds(rows)
    p2 = plt.subplot(212)
    p2.set_title("Volume")
    p2.bar(x,vols)

    plt.show()
    #pylab.plot(rows)
    #pylab.show()


import os
def brun(*args):
    #print(list(args))
    cmd = ' '.join(['beancounter'] + list(args))
    os.system(cmd)
    
def addback(ticker):
    brun("addstock", ticker)
    brun("backpopulate", ticker, '--prevdate', '"10 years ago"')

#brun("hello", "world")
#!beancounter addstock FGP.L
#!beancounter backpopulate FGP.L --prevdate "2 years ago"

def stuff():
    addback("KGF.L")
    dfsp = pd.read_sql("SELECT * FROM stockprices ORDER BY date", bean.db)
    dfsp.head()
    sp = dfsp.loc[dfsp['symbol'] == 'KGF.L']
    sp.head()

def csv_stuff():
    import csv
    import io
    strm = io.StringIO(r.text)
    with strm as cfile:
        rdr = csv.reader(cfile)
        for row in rdr:
            print(row)

if __name__ == "__main__":
    main()

