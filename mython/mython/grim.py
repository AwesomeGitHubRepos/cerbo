# yield and momentum filter
# http://bit.ly/ewaBY1

from operator import *

import tabstats

t = tabstats.Table()
t.read_csv("sh.csv")

t.rename_col('Yield', 'yld')
t.rename_col('RS_6Month', 'rs6')
t.rename_col('Gearing_Tangible', 'tgear')

t.col_percentiles('yld')
t.col_percentiles('rs6')
t.hide_col('yld%')
t.hide_col('rs6%')
t.hide_col('Sector')


t.drop('yld%', lt, 50)
t.drop('ROE', lt, 12)
t.drop('rs6', lt, 0)
t.drop('tgear', eq, '')
t.drop('tgear', ge, 100)

t.show()
