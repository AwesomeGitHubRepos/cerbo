import csv
import glob
import shlex

print "hello world"
print "how are you?"
fname = '../int/calcs/AAL'
#r = csv.reader(open('../int/calcs/AAL', 'rb'), delimiter=' ')
#rows = [row for row in r]
#print rows
#help(csv)
#w = 

def read_file(fname):
    rows = []
    for row in file(fname).readlines():
        row = row.strip()
        row = shlex.split(row)
        #print row
        #assert( len(row) == 3)
        rows.append(row)
    return zip(*rows) # return the transpose

hdr = None
data = []
for fname in glob.glob('../int/calcs/*'):
    #print fname
    contents = read_file(fname)
    hdr = list(contents[0])
    values = list(contents[1])
    data.append(values)
data = [hdr] + data
w = csv.writer(open('../int/misc/calcs.csv', 'wb'))
w.writerows(data)
    

