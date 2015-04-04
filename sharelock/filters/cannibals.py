# coding: utf-8
"Find the cannibals - ie where the number of shares has reduced"

import pandas

#from sharelock import *
import sharelock

df = sharelock.read_calcs()

# quality filter
df1 = df[['EPIC', "EE", 'NSHG']]
df2 = df1[(df.NSHG < 1)]
df3 = df2.sort_index(by="EE")
sharelock.prdf(df3)

#df1 = df[['EPIC', 'OPM1', 'EPS1', 'EPG01']]
#df2 = df1[df1.OPM1 > 0]
#df3 = df2[df2.EPS1 > 0]
#df4 = df3[df3.EPG01 > 0]
#prdf(df4)


