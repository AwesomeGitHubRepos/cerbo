"""
Regularise a CSV file for use in Fortran.
Uses as input ~/Downloads/StatsList.csv
Outputs to directory ~/.fortran
Deletes all the files in there first
Then, for each column in the CSV file, create a file in ~/.fortran
named after the column.
Each row in the file is the value in the column. Blank entries are given
the value NaN.
Finally, file _count is created containing the number of rows you should
read in.
"""
import os
import os.path
import shutil

import csvmc

#columns = ['RS_6Month', 'FTSE_Index']
data = csvmc.read_dict(os.path.expanduser('~/Downloads/StatsList.csv'))

outdir = os.path.expanduser("~/.fortran")
if not os.path.exists(outdir): os.makedirs(outdir)
for f in os.listdir(outdir):
    full = os.path.join(outdir, f)
    if os.path.isfile(full): os.remove(full)
#exit(0)



fp_count = open(outdir + "/" + "_count", "w")
count = len(data)
fp_count.write("{0}\n".format(count))
fp_count.close()

keys = data[1].keys()
keys = filter(lambda x: x is not None, keys)
fps = {}
for k in keys:
    print k
    fname = outdir + "/" + k
    fps[k] = open(fname, "w")

for row in data:
    for k in keys:
        v = row[k]
        if len(v) == 0: v = "NaN"
        fps[k].write(v + "\n")


for k in keys: fps[k].close()
print count

#print data[1].keys()
