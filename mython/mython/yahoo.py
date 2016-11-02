import sys

import requests
import parsedatetime # sudo pip3 install parsedatetime / yaourt python-parsedatetime



def ticker_filename(ticker):
    "Determine a filename for a ticker"
    dout = os.path.expanduser("~/.fortran")
    if not os.path.exists(dout): os.makedirs(dout)
    fname = dout + "/"
    #fname = "{0}/.fortran/{1}".format(
    #ticker = ticker.upper()
    fname += ticker
    return fname


def get_url(url):
    r = requests.get(url)
    r.connection.close()
    return r.text

def get_decade(ticker):
    """Get 10 years worth of data for a ticker  from Yahoo Finance.
        

    Args:
        ticker: ticker symbol to fetch. E.g. VOD.L for Vodafone, HYH for Halyard Health.
    """
    
    def dvals(human):
        d = parsedatetime.Calendar().parse(human)[0]
        # note we return month -1 due to the way Yahoo accepts months
        return d.tm_year , d.tm_mon -1 , d.tm_mday

    y0, m0, d0 = dvals("10 years ago")
    y1, m1, d1 = dvals("today")
    dfmt = "a={0}&b={1}&c={2}&d={3}&e={4}&f={5}"
    dstr = dfmt.format(m0, d0, y0, m1, d1, y1)
    ufmt = "http://ichart.finance.yahoo.com/table.csv?s={0}&{1}&g=d&ignore=.csv"   
    url = ufmt.format(ticker, dstr)
    print(url, file = sys.stderr)
    txt = get_url(url)
    
    lines = txt.splitlines()
    hdr = lines[0]
    data = lines[1:]
    data.reverse()
    out_text = "\n".join( [hdr] + data)

    return out_text

    if fout is None: fout = ticker_filename(ticker)
    open(fout,"w").write(out_text)



def yahoo_main():
    print("Seriously, just use ydec instead", file=sys.stderr)
    sym = sys.argv[1]
    print(get_decade(sym))
    
if __name__ == "__main__": yahoo_main()
