import datetime

#import pg # sudo apt-get install python-pygresql
import psycopg2 # sudo apt-get install python-psycopg2

#con = pg.connect('beancounter')
#cur = con.cursor()
#cur.execute("DELETE * from fxprices")

conn = psycopg2.connect("dbname='beancounter'")
cursor = conn.cursor()
cursor.execute("DELETE FROM fxprices;")
#cursor.commit()

cursor = conn.cursor()
numdays = 300
base = datetime.datetime.today()
dateList = [ base - datetime.timedelta(days=x) for x in range(0,numdays) ]
for d in dateList:
    dstamp = d.strftime("%y-%m-%d")
    sql =  "INSERT INTO fxprices VALUES ( 'P', '{0}', 0.01, 0.01, 0.01, 0.01, 0.01, 0.00);".format(dstamp)
    #print sql
    cursor.execute(sql)

conn.commit()
