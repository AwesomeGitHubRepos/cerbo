# module for performing stats on tables

import csv
#from operator import *
import types

def genint(n = None):
    'Generate integers, up to an including a max value of N'
    i = 0
    if n is None:
        while True:
            yield i
            i += 1
    else:
        while i <=n:
            yield i
            i += 1
    
class Table:
    def __init__(self):
        self.headers = []
        self.data = []
        self.hidden_cols = set()

    def append_col(self, name, values):
        self.headers.append(name)
        #print values
        data = []
        for row, v in zip(self.data, values):
            row.append(v)
            data.append(row)
        self.data = data

    def hide_col(self, name):
        'Suppress a column from an output'
        self.hidden_cols.add(name)

    def read_csv(self, filename):
        rdr = csv.reader(open(filename))
        table = [row for row in rdr]
        self.headers = table[0]
        self.data = []
        for row in table[1:]:
            processed_row = []
            for datum in row:
                try:
                    v = float(datum)
                except ValueError:
                    v = datum
                processed_row.append(v)
            self.data.append(processed_row)

    def col_index(self, name):
        '''Return a numerical index for the NAME of a column in a table.
        Returns None is column name not found'''
        col_num = None
        for i in self.col_range():
            if self.headers[i] == name: col_num = i
        return col_num

    def col(self, name):
        'Extract a named column'

        col_num = self.col_index(name)
        if col_num is None: return []

        result = [row[col_num] for row in self.data]
        return result
        
    def num_cols(self):
        'Number of columns in the table'
        return len(self.headers)

    def col_range(self):
        'Return a range of column numbers'
        return range(self.num_cols())

    def drop(self, name, op, value):
        'Drop those rows from column NAME whose VALUES satisfy OP'
        data = []
        cidx = self.col_index(name)
        for row in self.data:
            v = row[cidx]
            if not op(v, value):
                data.append(row)
        self.data = data

    def rename_col(self, old_name, new_name):
        'Rename a column header'
        for i in self.col_range():
            if self.headers[i] == old_name:
                self.headers[i] = new_name

    def col_percentiles(self, name):
        'Create percentiles for a table column'
        
        # sort the column by values
        unsorted = []
        col_vals = self.col(name)
        #print col_vals
        i = 0
        for v in col_vals:
            unsorted.append([i, v])
            i +=1
        #print unsorted
        ranked = sorted(unsorted, key = lambda x: x[1])
        #print ranked

        #assign a ranking
        list2 = []
        for row, i in zip(ranked, genint()):
            row.append(i)
            list2.append(row)
        #print list2

        #now put the list back into the original order
        list3 = sorted(list2, key = lambda x: x[0])
        #print list3

        #create a list of percentiles
        num_rows = float(len(list3))
        list4 = [ 100.0*float(row[2])/num_rows for row in list3]

        self.append_col(name + "%", list4)
        
    def order(self, colname):
        def func(a, b): return cmp(a[self.col_index(colname)] , b[self.col_index(colname)])
        self.data.sort(func)
        

        
    def show(self):
        def fmt_val(v):
            if type(v) is types.StringType:
                fmt = '{0:8}'
            elif type(v) is types.IntType:
                fmt = '{0:8}'
            elif type(v) is types.FloatType:
                fmt = '{0:>8.2f}'
            else:
                fmt = '{0:8}'
            text = fmt.format(v)
            return text
            
        def print_row(row):
            text = []
            for i, v in zip(genint(), row):
                if not (self.headers[i] in self.hidden_cols):
                    text.append(fmt_val(v))
            line = " ".join(text)
            print line

        print_row(self.headers)
        for row in self.data:
            print_row(row)



if __name__ == "__main__":
    t = Table()
    t.read_csv("sh.csv")

    # some renaming conveniences
    t.rename_col('Yield', 'yld')
    t.rename_col('RS_6Month', 'rs6')
    t.rename_col('Gearing_Tangible', 'tgear')

    # hide some columns
    t.hide_col('yld%')
    t.hide_col('ROE%')

    # work out percentiles (do this BEFORE you start dropping rows)
    t.col_percentiles('yld')
    t.col_percentiles('ROE')


    # drop undesirable items
    t.drop('yld%', lt, 50)
    t.drop('rs6', le, 0)
    t.drop('ROE%', lt, 50)
    t.drop('ROE', eq, '')
    #t.drop('tgear', eq, '') # probably has negative equity

    t.show()



