import datetime
import pprint
import os

import psycopg2 # sudo apt-get install python-psycopg2

import csvmc

conn = psycopg2.connect("dbname='beancounter'")
cursor = conn.cursor()
cursor.execute("DELETE FROM portfolio;")
conn.commit()

def add(e):
    
    fmt = "beancounter addportfolio {0}.L:{1}:P:hl:mc:{2}:{3}"
    sym = e['sym']
    dstamp = e['dstamp']
    dstamp = dstamp.replace('-', '')
    cmd = fmt.format(sym, e['qty'], e['unit'], dstamp)
    #sym = e['sym']
    # if len(sym)<2: sym += "."
    #sym += ".L"    
    print cmd
    os.system(cmd)

def main():
    etrans = csvmc.read_dict("/home/mcarter/docs/accts/int/csv/etransa.csv")
    for e in etrans: 
        if e['folio'] == "ut": continue
        add(e)
    # pprint.pprint(etrans)

if __name__ == "__main__": main()
