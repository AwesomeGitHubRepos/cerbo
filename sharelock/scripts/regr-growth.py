# coding: utf-8
# correlate PE and EV/EBITDA with Growth rates

import matplotlib
import pandas

from sharelock import *


#df = pandas.read_csv('/home/mcarter/repos/nokilli/sharelock/int/misc/calcs.csv')
df = loadclean()


# quality filter
#df1 = df[['EPIC', 'EE', 'EPG01', 'EPG02']]
df1 = df
df1 = df1[(df1.PER > 0) & (df1.PER < 25)]
df1 = df1[(df1.EPS_Growth_Projected > -10)]
df1 = df1[(df1.EPS_Growth_Projected < 25)]
#df3 = df2.sort_index(by="EE")

# prdf(df)
#print df1

#print pandas.ols(y = df1['EV_EBITDA'], x = df1['EPS_Growth_Projected'])
#df1.plot(x = df1['EPS_Growth_Projected'], y = df1['PER'])

#print pandas.ols(y = df['EV_EBITDA'], x = df['EPS_Growth_Projected'])
#matplotlib.pyplot.show()

for per in range(3, 20):
    per1 = per + 1
    df1 = df[(df.PER >= per) & (df.PER < per1)]
    rate = df1['EPS_Growth_Projected'].median()
    print per, rate
