# coding: utf-8
import pandas

#from sharelock import *
import sharelock

df = sharelock.read_calcs()
df = df[df.DIVI == "#t"]
df = df[['EPIC']]
df = df.sort(['EPIC'])
#df = df.drop(['index'])
#df.index = None
df = df.reset_index(drop = True)
sharelock.prdf(df)
#print df.columns


