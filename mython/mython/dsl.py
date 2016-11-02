from collections import namedtuple
from enum import Enum
from pprint import pprint
#import pprint

from pyparsing import *

lisp = """
(class yahoo)
(reverse true)
(col "Adj Close" (as aclose))
(col Close (as closeprice))
(col Date (as dstamp) (type S11))
"""

source = """
class yahoo 

reverse
reverse

col "Adj Close" as aclose .
col Close as closeprice .
col Date as dstamp type S11 .
#col Volume as vol .
col yuk .
col baz type foo .
.
"""

Type = Enum("Type", "cls opt")

Class = namedtuple('Class', ['name', 'opts', 'cols']) #, verbose = True)
Col   = namedtuple('Col', ['src', 'dest', 'opts'])

#class_name = Word(alphas)

def parse_spec():
    global source
    class_opts = ZeroOrMore(Group("reverse")) #.setparseAction(lambda t: [Type.opt, 'reverse'])
    class_opts.setParseAction(lambda t: t[0])

    col_name = (QuotedString('"', unquoteResults = True) | Word(alphanums))# .asString() # (Word(alphas) | QuotedString('"'))
    #col_name = Word(alphas)
    col_name.setParseAction(lambda t: t[0])
    #col_type = (Suppress("type") + Word(alphanums))
    col_opts = (Optional("as" + col_name, default = None) + Optional("type" + col_name, default = None))
    col_opts.setParseAction(lambda t: ("as", t[1], "type", t[2]))
    col_spec = ("col" + col_name + Optional(col_opts, default = None) + ".")
    col_spec.setParseAction(lambda t: Col(t[1], t[1], t[2]))
    col_specs = Group(OneOrMore(col_spec))

    comment = "#" + restOfLine

    class_spec = ("class" + Word(alphas) + class_opts + col_specs + ".")
    class_spec.setParseAction(lambda t: Class(t[1], t[2], t[3]))
    class_spec.ignore(comment)

    rtree = class_spec.parseString(source)

    #schema = { 'reverse' : False, 'columns' : [] }

    #assert(raw_tree[0] == 'class')
    #for node in raw_tree:
    #    print(node)

    #schema = raw_tree
    return rtree



def test():
    rtree = parse_spec()
    #print(rtree.dump())
    if False:
        for n in rtree:
            if type(n) is Class:
                print("Found class, name:", n.name)
                print("opts:", n.opts)
                print('cols:')
            #for c in n.cols.expr:
            #    print(c.__dict__)
            #for col in n.cols:
            #    print(col)
            #pprint(n.cols.__dict__)
            pprint(n.cols)
    #pprint(type(rtree[0].cols[0].__dict__))
    #print(type(rtree[0]) is Class)
    #n, o, cols = rtree[0]
    #for c in cols:
    #    print(c.dest)
    #print(n, o, c)
    #pprint(rtree.cols)
    pprint(rtree)

if __name__ == "__main__": test()
