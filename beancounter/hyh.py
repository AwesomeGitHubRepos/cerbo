from datetime import date

#import psycopg2
#import pylab
import matplotlib.pyplot as plt

import mython.listmc

import beancounter


def main():
    db = beancounter.open_db()
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

if __name__ == "__main__":
    main()
