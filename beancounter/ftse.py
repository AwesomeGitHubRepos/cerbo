import numpy.fft
import psycopg2
import pylab

from mython.listmc import firsts

db = psycopg2.connect(database = 'beancounter', user = 'mcarter')

#(connect-toplevel "beancounter" "mcarter" "" "localhost")

cur = db.cursor()
sql = "select day_close from stockprices where symbol='^FTSE' " + \
"order by date asc"
#print(sql)
cur.execute(sql)
rows = cur.fetchall()
db.close()
closes = firsts(rows)
trans = numpy.fft.rfft(closes)
# not sure if the following is accurate - shift it?
freqs = list(map(abs, trans))

def show(f):
    pylab.plot(range(len(f)), f)    
    pylab.show()

show(freqs[0:200])
