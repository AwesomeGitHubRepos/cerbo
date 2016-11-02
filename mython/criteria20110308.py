# see onenote "Share selection criteria" to see how it works

from operator import *

import tabstats

t = tabstats.Table()
t.read_csv("stats.csv")

t.rename_col('Yield', 'yld')
t.drop('yld', eq, 0)
t.col_percentiles('yld')
t.drop('yld%', lt, 67)

t.rename_col('Div_Cover', 'dcov')
t.col_percentiles('dcov')
t.drop('dcov%', lt, 67)


t.show()
