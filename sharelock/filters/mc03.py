# coding: utf-8
import pandas

#from sharelock import *
import sharelock

df = sharelock.read_calcs()

# quality filter
df1 = df[['EPIC', 'EE', 'EPG01', 'EPG02']]
df2 = df1[(df.EPG01 > 2) & (df.EPG02 < 3)]
df3 = df2.sort_index(by="EE")
sharelock.prdf(df3)

#df1 = df[['EPIC', 'OPM1', 'EPS1', 'EPG01']]
#df2 = df1[df1.OPM1 > 0]
#df3 = df2[df2.EPS1 > 0]
#df4 = df3[df3.EPG01 > 0]
#prdf(df4)


