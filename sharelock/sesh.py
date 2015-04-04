# coding: utf-8
from sharelock import *
df = loadclean()
df[df.EPIC == "JD."].to_string()
df[df.Sub_Sector == "APPAREL RETAILERS"].to_string()
df1 = df[df.Sub_Sector == "APPAREL RETAILERS"]
print df1
df1.to_string()
df2 = df1[['EPIC', 'MarketCap', 'EV_EBITDA']]
df2.to_string()
print df2.to_string()
df2.mean()
