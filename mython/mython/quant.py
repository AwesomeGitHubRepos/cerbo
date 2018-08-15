"""
Quant library for stock price analysis. 

Supercedes module bean.

Example usage is below.

.. highlight:: python
>>> get_decade("VOD.L")
Created file /home/mcarter/.fortran/VOD.L


Hope that helps.
"""

import argparse
import os.path


import matplotlib.dates as mdates # sudo apt-get install python3-matplotlib python-matplotlib-doc
import numpy as np
print("Disabling pandas at 02-Dec-2015. Seems it has a bug. load_to_pandas() won't work")
#import pandas as pd # sudo apt-get install python3-pandas



#print("Similar problem with a mython.times import")
import mython.times




def load_to_pandas(ticker, filename = None):
    """Load a historical CSV file to pandas. filename = None implies to use the
    default file path specification"""
    if filename is None:
        filename = ticker_filename(ticker)
    filename = os.path.expanduser(filename)
    df = pd.io.parsers.read_csv(filename)
    if "Date" in df.columns:
        df.sort_values(by = ['Date'], inplace=True)
        # Date is a string rather than a date object:
        df['DateObj'] = df['Date'].map(mython.times.str_to_date)
    if "Close" in df.columns:
        cls = np.array(df.Close)
        rsi14 = relative_strength(cls, 14)
        df['rsi14'] = rsi14
        df["dma50"] = sma(cls, 50)
        df['pv50'] = df.Close / df.dma50

    return df

def cli_main():
    print("You probably want to use the yahoo module instead")
    exit(1)
    p = argparse.ArgumentParser("Quant library")
    p.add_argument('--decade', dest = "decade", help="download ticker data for a decade")
    p.add_argument('-o', dest= "outfilename", help="output to file")    
    args = p.parse_args()
    #print(type(args.decade))
    if args.outfilename: args.outfilename = os.path.expanduser(args.outfilename)
    if args.decade: get_decade(args.decade.upper(), args.outfilename)


years    = mdates.YearLocator()   # every year
months   = mdates.MonthLocator()  # every month
quarterly = mdates.MonthLocator([1, 4, 7, 10])
yearsFmt = mdates.DateFormatter('%Y')

def add_plot_grid(ax):
    """Add major and minor x and y axes grids to a matplotlib.axes.AxesSubplot"""
    ax.xaxis.grid(True, which = 'minor')
    ax.xaxis.grid(True, which = 'major')
    ax.yaxis.grid(True, which = 'minor') 
    ax.yaxis.grid(True, which = 'major') 

def date_axes_quarterly(ax):
    ax.xaxis.set_major_locator(years)
    ax.xaxis.set_minor_locator(quarterly)
    add_plot_grid(ax)


def relative_strength(prices, n=14):
    """
    compute the n period relative strength indicator
    http://stockcharts.com/school/doku.php?id=chart_school:glossary_r#relativestrengthindex
    http://www.investopedia.com/terms/r/rsi.asp
    Source copied from:
    http://matplotlib.org/examples/pylab_examples/finance_work2.html
    Untested
    """

    deltas = np.diff(prices)
    seed = deltas[:n+1]
    up = seed[seed>=0].sum()/n
    down = -seed[seed<0].sum()/n
    rs = up/down
    rsi = np.zeros_like(prices)
    rsi[:n] = 100. - 100./(1.+rs)

    for i in range(n, len(prices)):
        delta = deltas[i-1] # cause the diff is 1 shorter

        if delta>0:
            upval = delta
            downval = 0.
        else:
            upval = 0.
            downval = -delta

        up = (up*(n-1) + upval)/n
        down = (down*(n-1) + downval)/n

        rs = up/down
        rsi[i] = 100. - 100./(1.+rs)

    return rsi


def shift_right(ser, n):
    arr = np.zeros(len(ser))
    for i in range(len(ser)):
        if i<n: arr[i] = np.nan
        else: arr[i] = ser[i -n]
    return arr

def relval(arr, n):
    a = np.array(arr)
    return a/shift_right(a, n)

def sma(alist, n = 50):
    """Simple moving average"""
    num = len(alist)
    result = np.zeros(num)
    for idx in range(num):
        i0 = max(0, idx - n+1 )
        #print(i0, idx)
        result[idx] = np.mean(alist[i0:idx+1])
    return result

def quantiles(df_column, ntiles=10):
    """
    Returns the quantiles in NTILES buckets
    Return type: pandas.core.series.Series
    
    Example usage:

    .. highlight:: python
    >>> import pandas
    >>> df = pd.io.parsers.read_csv('/home/mcarter/.fortran/StatsList.csv')
    >>> quantile(df['RS_6Month'], 5)
    0.0    -75.730
    0.2     -6.872
    0.4      6.156
    0.6     15.468
    0.8     28.872
    1.0    123.500
    Name: RS_6Month, dtype: float64

    
    The implementation is fairly simple:

    .. highlight:: python
    >>> qs = [i/ntiles for  i in range(ntiles+1)]
    >>> return df_column.quantile(qs)

    """
    qs = [i/ntiles for  i in range(ntiles+1)]
    return df_column.quantile(qs)    


if __name__ == "__main__": cli_main()
